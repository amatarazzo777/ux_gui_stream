/*
 * This file is part of the ux_gui_stream distribution
 * (https://github.com/amatarazzo777/ux_gui_stream).
 * Copyright (c) 2020 Anthony Matarazzo.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @author Anthony Matarazzo
 * @file ux_display_context.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief
 */
#include <ux_compile_options.h>
#include <ux_base.h>
#include <ux_hash.h>
#include <ux_system_error.h>
#include <ux_visitor_interface.h>
#include <ux_enums.h>
#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>
#include <ux_pipeline_memory.h>
#include <ux_display_visual.h>
#include <ux_display_unit_base.h>
#include <ux_event_listeners.h>
#include <ux_os_window_manager_event_base.h>
#include <ux_os_window_manager_base.h>

#include <ux_os_linux_xcb_event.h>
#include <ux_os_linux_xcb_window_manager.h>

#include <ux_display_context.h>

/**
 * @fn display_context_t operator =&(const display_context_t&)
 * @brief
 *
 * @param other
 * @return
 */
uxdevice::display_context_t &uxdevice::display_context_t::operator=(
  const uxdevice::display_context_t &other) {
  hash_members_t::operator=(other);
  system_error_t::operator=(other);
  pipeline_memory_t::operator=(other);

  regions_storage = other.regions_storage;
  surface_requests_storage = other.surface_requests_storage;

  return *this;
}

/**
 * @internal
 * @brief The routine checks the system for render work which primarily arrives
 * to the thread via the regions list. However, when no official work exists,
 * the condition variable cvRenderWork is placed in a wait state. The condition
 * may be awoke by calling the routine state_notify_complete().
 * @return bool - true - work exists, false none.
 */
bool uxdevice::display_context_t::surface_prime() {
  bool bRet = false;

  // no surface allocated yet, TODO refine surface_fn to provide auto return -
  // concepts20
  window_manager->surface_fn([&](auto surface) { bRet = surface != nullptr; });

  if (!bRet)
    return bRet;

  // determine if painting should also occur
  bRet = state();
  if (bRet)
    return bRet;

  // wait for render work if none has already been provided.
  // the state routines could easily produce region rectangular information
  // along the notification but do not. The user should call notify_complete.
  if (!bRet) {
    std::unique_lock<std::mutex> lk(render_work_mutex);
    render_work_condition_variable.wait(lk);
    lk.unlock();
    bRet = true;
  }

  return bRet;
}
/**
 * @internal
 * @brief The routine provides the synchronization of the xcb cairo surface and
 * the video system of xcb.
 */
void uxdevice::display_context_t::flush() { window_manager->video_flush(); }

/**
 * @internal
 * @brief The routine
 */
void uxdevice::display_context_t::device_offset(double x, double y) {
  window_manager->surface_fn(
    [&](auto surface) { cairo_surface_set_device_offset(surface, x, y); });

  state(0, 0, window_manager->window_width, window_manager->window_height);
}
/**
 * @internal
 * @brief The routine
 */
void uxdevice::display_context_t::device_scale(double x, double y) {
  window_manager->surface_fn(
    [&](auto surface) { cairo_surface_set_device_scale(surface, x, y); });

  state(0, 0, window_manager->window_width, window_manager->window_height);
}

/**
 * @internal
 * @brief The routine is called by a client to resize the window surface. In
 * addition, other work may be applied such as paint, however those go into a
 * separate list as the operating system provides these message independently,
 * that is Configure window event and a separate paint rectangle.
 */
void uxdevice::display_context_t::resize_surface(const int w, const int h) {
  std::lock_guard lock(surface_requests_storage_mutex);
  if (w != window_manager->window_width || h != window_manager->window_height)
    surface_requests_storage.emplace_back(w, h);
}

/**
 * @internal
 * @fn void apply_surface_requests(void)
 * @brief The routine applies resize requests of a window. The underlying
 * cairo surface is sized with the very last one.
 */
void uxdevice::display_context_t::apply_surface_requests(void) {
  std::lock_guard lock(surface_requests_storage_mutex);

  if (surface_requests_storage.empty())
    return;

  auto flat = surface_requests_storage.back();
  surface_requests_storage.clear();

  window_manager->surface_fn([&](auto surface) {
    cairo_surface_flush(surface);
    cairo_xcb_surface_set_size(surface, flat.w, flat.h);
  });

  window_manager->window_width = flat.w;
  window_manager->window_height = flat.h;

  viewport_rectangle.width = (double)flat.w;
  viewport_rectangle.height = (double)flat.h;
}

/**
 * @internal
 * @brief The routine paints the surface requests. The background brush is
 * emitted first then plot routine is called.
 */
void uxdevice::display_context_t::render(void) {
  clearing_frame = false;
  bool bEmpty = false;
  context_cairo_region_t processing_region = {};
  cairo_region_t *current = {};

  /**
   * rectangle of area needs painting background first. these are sub areas
   * perhaps multiples exist because of resize coordinate_t. The information
   * is generated from the paint dispatch event. When the window is opened
   * render work will contain entire window */
  apply_surface_requests();

  // partitionVisibility();

  // detect any changes that have occurred
  for (auto n : viewport_on)
    if (n->has_changed())
      state(n);

  /// @brief setup while start.
  {
    std::lock_guard<std::mutex> lock(regions_storage_mutex);
    bool bEmpty = regions_storage.empty();
    if (!bEmpty) {
      processing_region = regions_storage.front();
      regions_storage.pop_front();
    }
  }

  while (!bEmpty) {

    /**
     * @brief os surface requests are ideally full screen block coordinate_t
     * when multiples exist, such as clear, set surface as well as objects
     * that fit within the larger bounds, simply continue as there is no
     * redraw needed. The logic is subtle yet when overlp in is found on
     * subsequent regions, they are skipped due to the larger one containing
     * the last painted area. That is the area that plot was send to paint.
     * (processing_region). However, current is only set if there were os
     * region blocks. Other region blocks, even if they do contain, will need
     * painting because of zorder - or order which place into the
     * display_list.
     *
     * Future development will redevelop this portion. Most likely this
     * processing will be changed to a quad tree structure that encompasses
     * visibility partitioning and returns a list of shared pointers to
     * iterate. This will be better than the iterative cycle now.
     */
    if (current) {
      cairo_region_overlap_t ovrlp =
        cairo_region_contains_rectangle(current, &processing_region.rect);
      if (ovrlp == CAIRO_REGION_OVERLAP_IN)
        continue;
    } else {
      if (processing_region.bOSsurface)
        current = cairo_region_reference(processing_region._ptr);
    }

    /**
     * @brief the draw_fn locks the primary cairo context while drawing
     * operations occur which is the lambda expression. brush_mutex is the color
     * source (or image). these blocks are distinct work items such as the one
     * below. It only sets the brush, and paints a background square. */
    window_manager->draw_fn([&](auto cr) {
      std::lock_guard lock(window_manager->background_brush_mutex);
      window_manager->background_brush.emit(cr);
      cairo_rectangle(cr, processing_region.rect.x, processing_region.rect.y,
                      processing_region.rect.width,
                      processing_region.rect.height);
      cairo_fill(cr);
      cairo_push_group(cr);
    });

    /// @brief plot has other combinations of mutex locks. E.g. for
    /// regions_mutex, on_screen_mutex, surface_mutex, cr_mutex
    plot(processing_region);

    /// @brief alerts cairo that group has ended and this should paint.
    window_manager->draw_fn([](auto cr) {
      cairo_pop_group_to_source(cr);
      cairo_paint(cr);
    });

    /// @brief flush causes immediate update to video. Perhaps this could be
    /// adjusted to 60fps. The function has other combinations of mutex locks
    flush();

    /// @brief process surface requests such as window resizing. Internally
    /// has distinct mutex locks on the surface_mutex, surface_requests_mutex
    /// and cr_mutex.
    apply_surface_requests();

    // iteration to setup next plot area
    {
      std::lock_guard lock(regions_storage_mutex);
      bEmpty = regions_storage.empty();
      if (!bEmpty) {
        processing_region = regions_storage.front();
        regions_storage.pop_front();
      }
    }

    /// @brief is the frame is being cleared in another thread, just quite
    /// render operation.
    if (clearing_frame) {
      clearing_frame = false;
      break;
    }
  }

  if (current)
    cairo_region_destroy(current);
}

/**
 * @internal
 * @param std::shared_ptr<display_visual_t> _obj
 * @brief The routine adds a drawing output object to the appropriate list, on
 * or offscreen. If the item is on screen, a region area paint is requested
 * for the object's area.
 */
void uxdevice::display_context_t::add_visual(
  std::shared_ptr<display_visual_t> object_ptr) {

  /** @brief Allow object to instantiate itself. The object will select the
  visitor_target_bits from the pipeline memory. As well, add any special
  pipeline memory parameters used to communicate to other objects. Such as
  context variables like PangoLayout *. The object sets its ink_area at this
  point.

  */
  object_ptr->emit(this);

  /** @brief perform intersection test with the viewport */
  object_ptr->intersect(viewport_rectangle);

  /** initialize the display_visual_t object to utilize the drawing pipeline.
   * Both base (fn, clipping) and cached(fn,clipping) are initialized;*/
  auto ptr_pipeline = std::dynamic_pointer_cast<pipeline_memory_t>(object_ptr);

  object_ptr->fn_base_surface = [&]() {
    object_ptr->fn_draw = [&]() { ptr_pipeline->pipeline_visit(this); };

    object_ptr->fn_draw_clipped = [&]() {
      window_manager->draw_fn([&](auto cr) {
        cairo_rectangle(cr, object_ptr->intersection_double.x,
                        object_ptr->intersection_double.y,
                        object_ptr->intersection_double.width,
                        object_ptr->intersection_double.height);
        ptr_pipeline->pipeline_visit(this);
        cairo_clip(cr);
      });

      /**@brief linkage of object invocation within the current
       * display_context_t. "this" internally to the object provides the link
       * to pipeline memory visitation. The function uses the window_manager
       * internally to reference cairo_t and other attributes. The
       * ptr_pipeline->pipeline_visit(this); function is distinct for each
       * visual object as the drawing operations, order and initialization are
       * encapsulated. */
      ptr_pipeline->pipeline_visit(this);

      window_manager->draw_fn([&](auto cr) { cairo_reset_clip(cr); });
    };
  };

  object_ptr->fn_cache_surface = object_ptr->fn_base_surface;

  // validate object
  if (!ptr_pipeline->pipeline_has_required_linkages())
    return; // not adding error objects

  if (object_ptr->overlap == CAIRO_REGION_OVERLAP_OUT) {
    std::lock_guard lock(viewport_off_mutex);
    viewport_off.emplace_back(object_ptr);

  } else {
    std::lock_guard lock(viewport_on_mutex);
    viewport_on.emplace_back(object_ptr);
    state(object_ptr);
  }
}
/**
 * @internal
 * @brief The routine scans the offscreen list to see if any are now visible.
 */
void uxdevice::display_context_t::partition_visibility(void) {}

/**
 * @internal
 * @brief The routine clears the display context.
 */
void uxdevice::display_context_t::clear(void) {
  clearing_frame = true;

  {
    std::scoped_lock(regions_storage_mutex, viewport_on_mutex,
                     viewport_off_mutex);
    regions_storage.remove_if([](auto &n) { return !n.bOSsurface; });
    viewport_on.clear();
    viewport_off.clear();
  }

  offsetx = 0;
  offsety = 0;

  pipeline_memory_clear();
  state(0, 0, window_manager->window_width, window_manager->window_height);
}
/**
 * @internal
 * @brief The routine sets the background surface brush.
 */
void uxdevice::display_context_t::surface_brush(painter_brush_t &b) {

  {
    std::lock_guard lock(window_manager->background_brush_mutex);
    window_manager->background_brush = b;
  }

  state(0, 0, window_manager->window_width, window_manager->window_height);
}

/**
 * @internal
 * @brief The routine accepts a drawing output object and adds the
 * associated render work with the object's coordinate_t. note
 * stateNotifyComplete must be called after this to inform the renderer
 * there is work.
 */
void uxdevice::display_context_t::state(std::shared_ptr<display_visual_t> obj) {
  std::lock_guard lock(regions_storage_mutex);
  std::size_t onum = reinterpret_cast<std::size_t>(obj.get());

  regions_storage.emplace_back(context_cairo_region_t(
    onum, obj->ink_rectangle.x, obj->ink_rectangle.y, obj->ink_rectangle.width,
    obj->ink_rectangle.height));
}

/**
 * @internal
 * @brief The routine adds a generalized region area paint for the rendered
 * to find. note stateNotifyComplete must be called after this to inform the
 * renderer there is work.
 */
void uxdevice::display_context_t::state(int x, int y, int w, int h) {
  std::lock_guard lock(regions_storage_mutex);
  regions_storage.emplace_back(context_cairo_region_t{false, x, y, w, h});
}

/**
 * @internal
 * @brief The routine adds a surface oriented painting request to the render
 * queue. the items are inserted first before any other so that painting of
 * a newly resized window area occurs first.
 */
void uxdevice::display_context_t::state_surface(int x, int y, int w, int h) {
  std::lock_guard lock(regions_storage_mutex);
  auto it = std::find_if(regions_storage.begin(), regions_storage.end(),
                         [](auto &n) { return !n.bOSsurface; });
  if (it != regions_storage.end())
    regions_storage.insert(it, context_cairo_region_t{true, x, y, w, h});
  else
    regions_storage.emplace_back(context_cairo_region_t{true, x, y, w, h});
}
/**
 * @internal
 * @brief The routine notifies the condition variable that work has been
 * requested and should immediately being to render. Having this as a
 * separate function provides the ability to add work without rendering
 * occurring. However, message queue calls this when a resize occurs.
 */
void uxdevice::display_context_t::state_notify_complete(void) {
  render_work_condition_variable.notify_one();
}

/**
 * @internal
 * @brief The routine returns whether work is within the system.
 */
bool uxdevice::display_context_t::state(void) {
  bool ret = {};

  {
    std::lock_guard lock(regions_storage_mutex);
    ret = !regions_storage.empty();
  }

  /** surface requests should be performed, the render function sets the
   * surface size and exits if no region work.*/
  if (!ret) {
    {
      std::lock_guard lock(surface_requests_storage_mutex);
      ret = !surface_requests_storage.empty();
    }
  }

  return ret;
}

/**
 * @internal
 * @fn plot
 * @param context_cairo_region_t &plotArea
 * @details Routine iterates each of objects that draw and tests if the
 * rectangle is within the region.
 */
void uxdevice::display_context_t::plot(context_cairo_region_t &plotArea) {
  bool bDone = false;

  /// @brief exit early if empty.
  {
    std::lock_guard<std::mutex> lock(viewport_on_mutex);
    if (viewport_on.empty())
      return;
  }
  /// @brief iterator used for sequence of viewport_on
  display_visual_list_t::iterator itUnit = {};
  /// @brief iterator to item.
  std::shared_ptr<display_visual_t> n = {};
  {
    std::lock_guard lock(viewport_on_mutex);
    itUnit = viewport_on.begin();
    n = *itUnit;
  }

  while (!bDone) {

    n->intersect(plotArea._rect);

    switch (n->overlap) {
    case CAIRO_REGION_OVERLAP_OUT:
      break;
    case CAIRO_REGION_OVERLAP_IN: {
      n->fn_draw();
    } break;
    case CAIRO_REGION_OVERLAP_PART: {
      n->fn_draw_clipped();
    } break;
    }

    /// @brief save the state as being rendered.
    n->state_hash_code();

    if (clearing_frame) {
      bDone = true;
      continue;
    }

    /// @TODO iterator might be bad if list is cleared.
    {
      std::lock_guard lock(viewport_on_mutex);
      itUnit++;
      bDone = itUnit == viewport_on.end();
    }
  }
}
