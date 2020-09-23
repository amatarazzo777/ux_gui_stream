/*
 * This file is part of the PLATFORM_OBJ distribution
 * {https://github.com/amatarazzo777/platform_obj). Copyright (c) 2020 Anthony
 * Matarazzo.
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
\author Anthony Matarazzo
\file uxdisplayunits.hpp
\date 9/7/20
\version 1.0

\brief emit implementations for objects.

\details

The modules extends the uxdevice namespace. The objects
provided are the base objects for which the caller may instantiate to
draw either text or line.

While most of these objects are parameters to drawing functions, the
implementation with in this file provides the functional logic. All
objects derive from the display_unit_t class which contains the virtual
abstracts named for the object created from the template class.
Objects that provide drawing operations can inherit from
the drawing_output_t base class which enables visibility query.


*/

#include "ux_device.hpp"

/**

\fn text_render_normal_t::emit

\param display_context_t &context

\brief uses cached glyph bitmap rendering. fastest method of display
using bitmap font cache data.

\details

 */
void uxdevice::text_render_normal_t::emit(display_context_t &context) {
  // the emit function removes the text_render_path_t from the display memory
  // as they are exclusive
  context.unit_memory_erase<text_render_path_t>();
}

/**

\fn text_render_path_t::emit

\param display_context_t &context

\brief outputs text using vector drawing operations. This enables
the custom outline and fill of text. Slower but more versatile.
Text can bend to follow a path if logic is added.

\details

 */
void uxdevice::text_render_path_t::emit(display_context_t &context) {
  context.unit_memory_erase<text_render_normal_t>();
}

/**

\fn surface_area_title_t
\param display_context_t &context

\brief Sets the window title.

 */
void uxdevice::surface_area_title_t::emit(display_context_t &context) {
  // set window title
  xcb_change_property(context.connection, XCB_PROP_MODE_REPLACE, context.window,
                      XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, value.size(),
                      value.data());
}

/**

\fn surface_area_brush_t
\param display_context_t &context

\brief Sets the background brysh.

 */
void uxdevice::surface_area_brush_t::emit(display_context_t &context) {
  context.surface_brush(*this);
}

/**

\class text_alignment_t
\brief sets the alignment mode when wrapping, left, right, center,
justified.

\details


 */
void uxdevice::text_alignment_t::emit(PangoLayout *layout) {
  PangoAlignment correlated_value = static_cast<PangoAlignment>(value);
  if (value == text_alignment_options_t::justified &&
      !pango_layout_get_justify(layout)) {
    pango_layout_set_justify(layout, true);
  } else if (pango_layout_get_alignment(layout) != correlated_value) {
    pango_layout_set_justify(layout, false);
    pango_layout_set_alignment(layout, correlated_value);
  }
}

/**

\class coordinate_t
\brief The class is used to hold a location and clipping width, height.

\details

 */

void uxdevice::coordinate_t::emit_relative(cairo_t *cr) {
  cairo_rel_move_to(cr, x, y);
}
void uxdevice::coordinate_t::emit_absolute(cairo_t *cr) {
  cairo_move_to(cr, x, y);
}
void uxdevice::coordinate_t::emit(PangoLayout *layout) {
  if (pango_layout_get_width(layout) != w * PANGO_SCALE)
    pango_layout_set_width(layout, w * PANGO_SCALE);

  if (pango_layout_get_height(layout) != h * PANGO_SCALE)
    pango_layout_set_height(layout, h * PANGO_SCALE);
}

/**

\class text_indent_t
\brief indentation of first line paragraph.

\details


 */
void uxdevice::text_indent_t::emit(PangoLayout *layout) {
  int pangoUnits = value * PANGO_SCALE;
  pango_layout_set_indent(layout, pangoUnits);
}

/**

\class text_ellipsize_t
\brief when text is longer than the defined box, "..."
  is shown. can be at beginning, middle, or end.

\details


 */
void uxdevice::text_ellipsize_t::emit(PangoLayout *layout) {
  pango_layout_set_ellipsize(layout, static_cast<PangoEllipsizeMode>(value));
}

/**

\class text_line_space_t
\brief space between the lines of text.

\details


 */
void uxdevice::text_line_space_t::emit(PangoLayout *layout) {
  pango_layout_set_line_spacing(layout, static_cast<float>(value));
}

/**

\class text_tab_stops_t
\brief when tabs occur, this defines the tab stops.

\details


*/
void uxdevice::text_tab_stops_t::emit(PangoLayout *layout) {
  PangoTabArray *tabs = pango_tab_array_new(value.size(), true);

  int idx = 0;
  for (auto &tabdef : value) {
    int loc = static_cast<int>(tabdef);
    pango_tab_array_set_tab(tabs, idx, PANGO_TAB_LEFT, loc);
  }
  pango_layout_set_tabs(layout, tabs);
  pango_tab_array_free(tabs);
}

/**

\class text_font_t::emit
\brief

\details


 */
void uxdevice::text_font_t::emit(PangoLayout *layout) {
  if (!font_ptr) {
    font_ptr = pango_font_description_from_string(description.data());
    if (!font_ptr) {
      std::string s = "Font could not be loaded from description. ( ";
      s += description + ")";
      error_report(s);
    }
  }

  const PangoFontDescription *internal_description =
      pango_layout_get_font_description(layout);
  if (!internal_description ||
      !pango_font_description_equal(internal_description, font_ptr))
    if (font_ptr)
      pango_layout_set_font_description(layout, font_ptr);
}

/**
\internal
\fn text_data_t::emit(layout)
\brief sets that text of the layout to the one stored withi nthe text_data_t
  object. The text_data_t object contains a text_data_storage_t which is a
  std::variant holding the information within a string, string_view or

*/
std::size_t uxdevice::text_data_t::hash_code(void) const noexcept {
  std::size_t __value = std::type_index(typeid(text_data_t)).hash_code();

  auto text_data_visitor = overload_visitors_t{
      [&](std::string s) { hash_combine(__value, s); },
      [&](std::string_view s) { hash_combine(__value, s); },
      [&](std::shared_ptr<std::string> ps) { hash_combine(__value, *ps); },
      [&](std::shared_ptr<std::string_view> ps) { hash_combine(__value, *ps); },
      [&](std::shared_ptr<std::stringstream> ps) {
        std::string stmp = ps->str();
        hash_combine(__value, stmp);
      }};

  std::visit(text_data_visitor, value);

  return __value;
}

/**
\internal
\fn text_data_t::emit(layout)
\brief sets that text of the layout to the one stored withi nthe text_data_t
  object. The text_data_t object contains a text_data_storage_t which is a
  std::variant holding the information within a string, string_view or

*/
void uxdevice::text_data_t::emit(PangoLayout *layout) {
  std::string_view sinternal = std::string_view(pango_layout_get_text(layout));
  auto text_data_visitor =
      overload_visitors_t{[&](std::string s) {
                            if (s.compare(sinternal) != 0)
                              pango_layout_set_text(layout, s.data(), -1);
                          },

                          [&](std::string_view s) {
                            if (s.compare(sinternal) != 0)
                              pango_layout_set_text(layout, s.data(), -1);
                          },

                          [&](std::shared_ptr<std::string> ps) {
                            if (ps->compare(sinternal) != 0)
                              pango_layout_set_text(layout, ps->data(), -1);
                          },

                          [&](std::shared_ptr<std::string_view> ps) {
                            if (ps->compare(sinternal) != 0)
                              pango_layout_set_text(layout, ps->data(), -1);
                          },

                          [&](std::shared_ptr<std::stringstream> ps) {
                            std::string stmp = ps->str();
                            if (stmp.compare(sinternal) != 0)
                              pango_layout_set_text(layout, stmp.data(), -1);
                          }};

  std::visit(text_data_visitor, value);
}

/**

\fn textual_render_storage_t::hash_code(void)

\brief hash code of the textual rendering. used to detect changes
that might occur within all of the dependent rendering parameters,

\details


 */
std::size_t uxdevice::textual_render_storage_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(textual_render_storage_t)),
               pango_layout_get_serial(layout), ink_rect.x, ink_rect.y,
               ink_rect.width, ink_rect.height, matrix.hash_code(),
               unit_memory_hash_code());
  return __value;
}

/**
\internal
\fn set_layout_options
\brief manages the layout options.

\details


 */
void uxdevice::textual_render_storage_t::set_layout_options(cairo_t *cr) {
  // create layout
  if (!layout)
    layout = pango_cairo_create_layout(cr);

  // get the serial number of all the current attributes.
  // if any of the emitter functions change the layout
  // the serial number changes.
  guint layoutSerial = pango_layout_get_serial(layout);

  // emit all items that effect pango objects as labeled
  // with attribute settings.
  unit_memory_visit(pango_visitor_t{cr, layout});

  // any changes
  if (layoutSerial != pango_layout_get_serial(layout)) {
    auto coordinate = unit_memory<coordinate_t>();
    pango_layout_get_pixel_extents(layout, &ink_rect, &logical_rect);
    int tw = std::min((double)logical_rect.width, coordinate->w);
    int th = std::min((double)logical_rect.height, coordinate->h);
    ink_rectangle = {(int)coordinate->x, (int)coordinate->y, tw, th};
    ink_rectangle_double = {(double)ink_rectangle.x, (double)ink_rectangle.y,
                            (double)ink_rectangle.width,
                            (double)ink_rectangle.height};

    has_ink_extents = true;
    pango_cairo_update_layout(cr, layout);
  }
}

/**
\internal
\fn create_text_shadow
\brief creates an image of the shadowed text

\details


 */
void uxdevice::text_shadow_t::handle_text_shadow(cairo_t *tocr,
                                                 coordinate_t &a) {
  // if the text has changed, recreate the shadow texture
  if (unit_memory<text_data_t>()->has_changed()) {
    shadow_buffer = draw_buffer_t{};
  }

  if (!shadow_buffer.rendered) {
    auto text_shadow = unit_memory<text_shadow_t>();
    shadow_buffer = draw_buffer_t{ink_rectangle_double.width + text_shadow->x,
                                  ink_rectangle_double.height + text_shadow->y};

    // create a buffer that can be rendering into using a cr
    // for the shadow
    cairo_move_to(shadow_buffer.cr, text_shadow->x, text_shadow->y);
    set_layout_options(shadow_buffer.cr);
    text_shadow->emit(shadow_buffer.cr);
    pango_cairo_show_layout(shadow_buffer.cr, layout);

    shadow_buffer.blur_image(text_shadow->radius);
  }
  // draw the shadow
  shadow_buffer.emit(tocr, a);
}

/**
\internal
\fn pipeline

\brief directly calls if exists or decides based upon rendering parameters set
and then calls function. The static

 */
void uxdevice::textual_render_storage_t::pipeline(cairo_t *cr,
                                                  coordinate_t &a) {
  /// @brief call cached function if set.
  if (pipeline_fn) {
    pipeline_fn();
    return;
  }

  /**
    \details The cached rendering function has not been established,
    create the text rendering pipeline consisting of individualized lambda
    functions.
  */

  /// @brief these steps are common in the first part of the textual rendering
  /// The visitor of the pango, and cairo operate in a sorted fashion to execute
  /// themselves within a rule based set of order. refinements within the stream
  /// can occur as successive execution happens. Initialization for example, and
  /// build up of graphics composite layers.
  text_render_pipeline.emplace_back([&]() {
    matrix.emit(cr);
    drawing_output_t::emit(cr);
    set_layout_options(cr);
  });

  // emit all cairo_coordinate_visitor_t based objects.
  // the items include only associated textual rendering cr
  // items as created by the linkages from the context.
  // // emit pango based visitors.

  // compute pipeline
  auto exec = unit_memory_to_staged_pipeline(cairo_coordinate_visitor_t{cr, a},
                                             pango_visitor_t{cr, layout});

  text_render_pipeline.splice(text_render_pipeline.end(), exec);

  /// @brief additional processing can also be inserted here. or
  /// may be inserted as a new unit_memory emitter accepting a visitor type.

  /// @brief set the pipeline drawing function to the one that will be used
  /// to set the proper values
  pipeline_fn = [&]() {
    // unroll text pipeline calls
    for (auto n : text_render_pipeline)
      n();
    error_check(cr);
  };

  /// @brief invoke.
  pipeline_fn();

  return;
}

/**
\internal
\fn emit
\param display_context_t &context

\brief creates linkages to the drawing functions and rendering parameters

\details


 */
void uxdevice::textual_render_t::emit(display_context_t &context) {
  using namespace std::placeholders;

  // create a linkage snapshot to the
  // shared pointers stored in unit memory
  // within the stream context. Only the ones that are labels attributes
  // visitor_unit_memory_textual_render_t are copied due to template
  // construction of unit_memory_storage_t;
  unit_memory_linkages(context);

  // check the context parameters before operating
  if (!((unit_memory<text_color_t>() || unit_memory<text_outline_t>() ||
         unit_memory<text_fill_t>()) &&
        unit_memory<coordinate_t>() && unit_memory<text_data_t>() &&
        unit_memory<text_font_t>())) {
    const char *s =
        "A textual_render_t object must include the following "
        "attributes: A text_color_t, text_outline_t or "
        " text_fill_t. As well, a coordinate_t, text and text_font_t object.";
    error_report(s);
    auto fn = [=](display_context_t &context) {};

    fn_base_surface = std::bind(fn, _1);
    fn_cache_surface = std::bind(fn, _1);
    fn_draw = std::bind(fn, _1);
    fn_draw_clipped = std::bind(fn, _1);
    return;
  }

  auto &coordinate = *unit_memory<coordinate_t>();

  // two functions provide mode switching for the rendering.
  // a cache surface is a new xcb surface that can be threaded in creation
  // but it is not yet implemented as such.

  // set the cache surface function
  fn_cache_surface = [=](display_context_t &context) {
    // if the item is already cached, return.
    if (bRenderBufferCached)
      return;

#if 0
    context.lock(true);
    set_layout_options(context.cr);
    UX_ERROR_CHECK(context.cr);
    context.lock(false);
#endif // 0

    internal_buffer = draw_buffer_t(ink_rect.width, ink_rect.height);

    coordinate_t a = coordinate;

    /**
    \brief because the image of the item will be drawn at coordinate 0,0
     in the off screen cache buffer, the possibility of bitmap image texturing
     must be translated to this coordinate space.
    */
    if (unit_memory<text_render_path_t>()) {
      if (unit_memory<text_fill_t>())
        unit_memory<text_fill_t>()->translate(-a.x, -a.y);
      if (unit_memory<text_outline_t>())
        unit_memory<text_outline_t>()->translate(-a.x, -a.y);
    } else {
      // text color must be set, so dependency on this not failing.
      unit_memory<text_color_t>()->translate(-a.x, -a.y);
    }

    // draw into 0,0 of buffer
    a.x = 0;
    a.y = 0;

    // execute the pipeline function
    pipeline(internal_buffer.cr, a);
    error_check(internal_buffer.cr);

    internal_buffer.flush();
    error_check(internal_buffer.rendered);

    // set drawing functions to emit this surface using the given matrix and
    // drawing options
    auto draw_fn = [=](display_context_t &context) {
      double tw, th;
      tw = std::min(ink_rectangle_double.width, coordinate.w);
      th = std::min(ink_rectangle_double.height, coordinate.h);

      auto coord = coordinate_t{coordinate.x, coordinate.y, tw, th};
      internal_buffer.emit(context.cr, coord);
    };
    auto fn_clipping = [=](display_context_t &context) {
      auto coord =
          coordinate_t{intersection_double.x, intersection_double.y,
                       intersection_double.width, intersection_double.height};
      internal_buffer.emit(context.cr, coord);
    };

    // the importance of the functors_lock here
    // is callable state of the drawing functions. The situations handled is
    // if either of these drawing functions are being called from the
    // rendering thread while these statements are changing them, a fault will
    // occur. The lock ensures that the transition from base rendering
    // services to a static textual image changeover does not cause a problem.
    // (time point multi core collision on read write of drawing function).
    functors_lock(true);
    fn_draw = std::bind(draw_fn, _1);
    fn_draw_clipped = std::bind(fn_clipping, _1);
    functors_lock(false);
    bRenderBufferCached = true;
  };

  // the base option rendered contains two functions that called using the
  // cairo API to the base surface context. One is for clipping and one
  // without. The
  auto fn_base = [&](display_context_t &context) {
    auto draw_fn = [&](display_context_t &context) {
      pipeline(context.cr, coordinate);
      evaluate_cache(context);
    };
    auto fn_clipping = [&](display_context_t &context) {
      cairo_rectangle(context.cr, intersection_double.x, intersection_double.y,
                      intersection_double.width, intersection_double.height);
      cairo_clip(context.cr);
      pipeline(context.cr, coordinate);
      cairo_reset_clip(context.cr);
      evaluate_cache(context);
    };
    functors_lock(true);
    fn_draw = std::bind(draw_fn, _1);
    fn_draw_clipped = std::bind(fn_clipping, _1);
    functors_lock(false);

    // when switching to base services, remove cache
    if (bRenderBufferCached) {
      internal_buffer = draw_buffer_t{};
      bRenderBufferCached = false;
    }
  };

#if 0
  context.lock(true);
  set_layout_options(context.cr);
  context.lock(false);

#endif // 0

  fn_base_surface = fn_base;
  fn_base_surface(context);

  is_processed = true;
}

/**
\internal
\fn pipeline

\brief create the image rendering pipeline

 */
void uxdevice::image_block_storage_t::pipeline(cairo_t *cr, coordinate_t &a) {
  /// @brief call cached function if set.
  if (pipeline_fn) {
    pipeline_fn();
    return;
  }

  /**
    \details The cached rendering function has not been established,
    create the image rendering pipeline consisting of individualized lambda
    functions. This functionality can provide image processing capabilities
    to produce visual effects. Using the emplace back to invoke operations
    on the image->block_ptr;
  */

  /// @brief these steps are common in the first part of the textual rendering
  image_render_pipeline.emplace_back([&]() { matrix.emit(cr); });

  /// compute pipeline
  /// @brief add to the end,
  /// staged pipeline simply means the sorting order
  /// see uxpipelineorder.hpp
  unit_memory_to_staged_pipeline(text_render_pipeline,
                                 cairo_coordinate_visitor_t{cr, a});

  /// @brief these steps are common in the first part of the textual rendering
  image_render_pipeline.emplace_back([&]() { image_block_ptr->emit(cr, a); });

  /// @brief set the pipeline drawing function to the one that will be used
  /// to set the proper values
  pipeline_fn = [&]() {
    // unroll text pipeline calls
    for (auto n : image_render_pipeline)
      n();
    error_check(cr);
  };

  /// @brief invoke.
  pipeline_fn();

  return;
}

/**
\internal
\brief
*/
void uxdevice::image_block_t::emit(display_context_t &context) {
  using namespace std::placeholders;

  if (is_loaded)
    return;

  // create a linkage snapshot to the
  // shared pointers stored in unit memory
  // within the stream context. Only the ones that are labels attributes
  // attribute_unit_memory_image_block_t are copied due to template
  // construction of unit_memory_storage_t;
  unit_memory_linkages(context);

  // validate required parameters
  if (!unit_memory<coordinate_t>() || description.size() == 0) {
    const char *s = "An image_block_t object must include the following "
                    "attributes. coordinate_t and an image_block_t name.";
    error_report(s);
    auto fn = [=](display_context_t &context) {};

    fn_base_surface = std::bind(fn, _1);
    fn_cache_surface = std::bind(fn, _1);
    fn_draw = std::bind(fn, _1);
    fn_draw_clipped = std::bind(fn, _1);
    return;
  }

  // set the ink area.
  coordinate_t &a = *unit_memory<coordinate_t>();

  auto fnthread = [&]() {
    image_block_ptr = std::make_shared<draw_buffer_t>(
        description, coordinate->w, coordinate->h);

    if (image_block_ptr->format == draw_buffer_format_t::none) {
      const char *s = "The image_block_t could not be processed or loaded. ";
      error_report(s);
      error_report(description);

    } else {
      ink_rectangle = {(int)a.x, (int)a.y, (int)a.w, (int)a.h};
      ink_rectangle_double = {(double)ink_rectangle.x, (double)ink_rectangle.y,
                              (double)ink_rectangle.width,
                              (double)ink_rectangle.height};
      has_ink_extents = true;
      is_loaded = true;
    }
  };

  fnthread();

  auto fn_cache = [&](display_context_t &context) {
    // set directly callable rendering function.
    auto fn = [&](display_context_t &context) {
      if (!is_valid())
        return;
      pipeline(context.cr, *unit_memory<coordinate_t>());
    };
    auto fn_clipping = [&](display_context_t &context) {
      if (!is_valid())
        return;

      // a.x, a.y,
      auto coords =
          coordinate_t{intersection_double.x, intersection_double.y,
                       intersection_double.width, intersection_double.height};
      pipeline(context.cr, coords);
    };
    functors_lock(true);
    fn_draw = std::bind(fn, _1);
    fn_draw_clipped = std::bind(fn_clipping, _1);
    functors_lock(false);
    bRenderBufferCached = true;
  };

  // two function provide mode switching for the rendering.
  // a cache surface is a new xcb surface that can be threaded in creation
  // base surface issues the drawing commands to the base window drawing cairo
  // context
  fn_cache_surface = fn_cache;
  fn_base_surface = fn_cache;
  fn_base_surface(context);

  is_processed = true;
  state_hash_code();
}

/**
\internal
\class function_object_t
\internal
\brief call previously bound function that was supplied parameter
 using std::bind with the cairo context.

\details

*/
void uxdevice::function_object_t::emit(cairo_t *cr) { value(cr); }

/**

\class option_function_object_t
\brief

\details


 */
void uxdevice::cairo_option_function_t::emit(cairo_t *cr) {
  //  value(context.cr);
}

/**
\internal
\brief
*/
void uxdevice::draw_function_object_t::emit(display_context_t &context) {
  using namespace std::placeholders;

  // get the drawing options for the context.
  auto options = context.unit_memory<cairo_option_function_t>();

  // the cache functions
  auto fnCache = [=](display_context_t &context) {
    // set directly callable rendering function.
    auto fn = [=](display_context_t &context) {
      drawing_output_t::emit(context.cr);
      value(context.cr);
    };

    auto fnClipping = [=](display_context_t &context) {
      drawing_output_t::emit(context.cr);

      cairo_rectangle(context.cr, intersection_double.x, intersection_double.y,
                      intersection_double.width, intersection_double.height);
      cairo_clip(context.cr);
      value(context.cr);
      cairo_reset_clip(context.cr);
      evaluate_cache(context);
    };

    functors_lock(true);
    fn_draw = std::bind(fn, _1);
    fn_draw_clipped = std::bind(fnClipping, _1);
    functors_lock(false);
    bRenderBufferCached = true;
  };

  // two function provide mode switching for the rendering.
  // a cache surface is a new xcb surface that can be threaded in creation
  // base surface issues the drawing commands to the base window drawing cairo
  // context
  fn_cache_surface = fnCache;
  fn_base_surface = fnCache;
  fn_base_surface(context);

  is_processed = true;
  state_hash_code();
}

/**

\class antialias_t
\brief

\details


*/
void uxdevice::antialias_t::emit(cairo_t *cr) {
  cairo_set_antialias(cr, static_cast<cairo_antialias_t>(value));
}

/**

\class line_width_t
\brief sets the line width when used during a stroke path operation.
This includes text and line drawing.

\details


 */
void uxdevice::line_width_t::emit(cairo_t *cr) {
  cairo_set_line_width(cr, value);
}

/**

\class line_cap_t
\brief

\details


*/
void uxdevice::line_cap_t::emit(cairo_t *cr) {
  cairo_set_line_cap(cr, static_cast<cairo_line_cap_t>(value));
}

/**

\class line_join_t
\brief

\details


*/
void uxdevice::line_join_t::emit(cairo_t *cr) {
  cairo_set_line_join(cr, static_cast<cairo_line_join_t>(value));
}
/**

\class miter_limit_t
\brief

\details


 */
void uxdevice::miter_limit_t::emit(cairo_t *cr) {
  cairo_set_miter_limit(cr, value);
}
/**

\class line_dash_storage_t - friend of line_deshes_t
\brief

\details


 */
void uxdevice::line_dashes_t::emit(cairo_t *cr) {
  cairo_set_dash(cr, value.data(), value.size(), offset);
}

/**

\class tollerance_t
\brief

\details


 */
void uxdevice::tollerance_t::emit(cairo_t *cr) {
  cairo_set_tolerance(cr, value);
}

/**

\class graphic_operator
\brief

\details


*/
void uxdevice::graphic_operator_t::emit(cairo_t *cr) {
  cairo_set_operator(cr, static_cast<cairo_operator_t>(value));
}

/**

\class arc_storage_t
\brief

\details


*/
void uxdevice::arc_t::emit(cairo_t *cr) {
  cairo_arc(cr, xc, yc, radius, angle1, angle2);
}

/**

\class negative_arc_t
\brief

\details


 */
void uxdevice::negative_arc_t::emit(cairo_t *cr) {
  cairo_arc_negative(cr, xc, yc, radius, angle1, angle2);
}

/**

\class curve_storage_t
\brief

\details


 */
void uxdevice::curve_t::emit_relative(cairo_t *cr) {
  cairo_rel_curve_to(cr, x1, y1, x2, y3, x3, y3);
}

void uxdevice::curve_t::emit_absolute(cairo_t *cr) {
  cairo_curve_to(cr, x1, y1, x2, y2, x3, y3);
}

/**

\class line_t
\brief

\details


 */
void uxdevice::line_t::emit_relative(cairo_t *cr) {
  cairo_rel_line_to(cr, x, y);
}

void uxdevice::line_t::emit_absolute(cairo_t *cr) { cairo_line_to(cr, x, y); }

/**

\class hline_t
\brief

\details


 */

void uxdevice::hline_t::emit_relative(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_rel_line_to(cr, value, 0);
  }
}

void uxdevice::hline_t::emit_absolute(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_line_to(cr, value, cury);
  }
}
/**

\class vline_t
\brief

\details


 */

void uxdevice::vline_t::emit_relative(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_rel_line_to(cr, 0, value);
  }
}

void uxdevice::vline_t::emit_absolute(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_line_to(cr, curx, value);
  }
}
/**

\class rectangle_t
\brief

\details


 */
void uxdevice::rectangle_t::emit(cairo_t *cr) {
  cairo_rectangle(cr, x, y, width, height);
}
/**

\class close_path_t
\brief

\details


 */
void uxdevice::close_path_t::emit(cairo_t *cr) { cairo_close_path(cr); }

/**

\class stroke_path_t
\brief

\details


*/
void uxdevice::stroke_path_t::emit(cairo_t *cr) {
  painter_brush_t::emit(cr);
  cairo_stroke(cr);
}

/**

\class fill_path_t
\brief

\details


 */
void uxdevice::fill_path_t::emit(cairo_t *cr) {
  painter_brush_t::emit(cr);
  cairo_fill(cr);
}

/**

\class stroke_fill_path_t
\brief

\details


 */
void uxdevice::stroke_fill_path_t::emit(cairo_t *cr) {
  stroke_brush.emit(cr);
  cairo_stroke_preserve(cr);
  fill_brush.emit(cr);
  cairo_fill(cr);
}

/**

\class mask_t
\brief

\details


 */
void uxdevice::mask_t::emit(cairo_t *cr) {}

/**

\class paint_t
\brief

\details


 */
void uxdevice::paint_t::emit(cairo_t *cr) {
  if (value == 1.0) {
    cairo_paint(cr);
  } else {
    cairo_paint_with_alpha(cr, value);
  }
}
/**

\class relative_coordinate_t
\brief

\details


 */
void uxdevice::relative_coordinate_t::emit(display_context_t &context) {
  context.unit_memory_erase<absolute_coordinate_t>();
}

/**

\class absolute_coordinate_t
\brief

\details


 */
void uxdevice::absolute_coordinate_t::emit(display_context_t &context) {
  context.unit_memory_erase<relative_coordinate_t>();
}
