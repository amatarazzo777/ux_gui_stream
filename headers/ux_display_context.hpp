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
\brief
*/
/**
\author Anthony Matarazzo
\file uxdisplaycontext.hpp
\date 5/12/20
\version 1.0
 \details CLass holds the display window context, gui drawing, cairo
 context, and provides an interface for threads running to
 invalidate part of the surface, resize the surface. The
 unit_memory_t class is within the public members and holds
 the state of the last used data parameters.

*/
#pragma once

/**
 *
 * \typedef visual_list_t
 * \brief used by the system to hold a list of items that draw.
 *
 */
namespace uxdevice {
typedef std::list<std::shared_ptr<display_visual_t>> visual_list_t;
typedef std::list<std::shared_ptr<display_visual_t>>::iterator
    visual_list_iter_t;
} // namespace uxdevice

class display_context_t
    : virtual public hash_members_t,
      public unit_memory_storage_t<visitor_unit_memory_display_context_t>,
      virtual system_error_t {
public:
  class context_cairo_region_t {
  public:
    context_cairo_region_t() = delete;
    context_cairo_region_t(bool bOS, int x, int y, int w, int h) {
      rect = {x, y, w, h};
      _rect = {(double)x, (double)y, (double)w, (double)h};
      _ptr = cairo_region_create_rectangle(&rect);
      bOSsurface = bOS;
    }
    context_cairo_region_t(std::size_t _obj, int x, int y, int w, int h)
        : obj(_obj) {
      rect = {x, y, w, h};
      _rect = {(double)x, (double)y, (double)w, (double)h};
      _ptr = cairo_region_create_rectangle(&rect);
      bOSsurface = false;
    }

    context_cairo_region_t(const context_cairo_region_t &other) {
      *this = other;
    }
    context_cairo_region_t &operator=(const context_cairo_region_t &other) {
      _ptr = cairo_region_reference(other._ptr);
      rect = other.rect;
      _rect = other._rect;
      obj = other.obj;
      bOSsurface = other.bOSsurface;
      return *this;
    }
    ~context_cairo_region_t() {
      if (_ptr)
        cairo_region_destroy(_ptr);
    }
    cairo_rectangle_int_t rect = cairo_rectangle_int_t();
    cairo_rectangle_t _rect = cairo_rectangle_t();
    cairo_region_t *_ptr = nullptr;
    std::size_t obj = 0;
    bool bOSsurface = false;
  };

public:
  display_context_t(void) {}

  display_context_t(const display_context_t &other) { *this = other; }

  // move constructor
  display_context_t(display_context_t &&other) noexcept {
    { *this = other; }
  }

  display_context_t &operator=(const display_context_t &other) {
    window_x = other.window_x;
    window_y = other.window_y;
    window_width = other.window_width;
    window_height = other.window_height;
    window_open = other.window_open;
    if (other.cr)
      cr = cairo_reference(other.cr);
    _regions = other._regions;
    _surfaceRequests = other._surfaceRequests;

    xdisplay = other.xdisplay;
    connection = other.connection;
    screen = other.screen;
    window = other.window;
    graphics = other.graphics;
    visualType = other.visualType;
    syms = other.syms;
    xcbSurface = other.xcbSurface;
    preclear = other.preclear;

    return *this;
  }

  visual_list_t viewport_off = {};
  std::atomic_flag drawables_off_readwrite = ATOMIC_FLAG_INIT;
#define VIEWPORT_OFF_SPIN                                                      \
  while (drawables_off_readwrite.test_and_set(std::memory_order_acquire))
#define VIEWPORT_OFF_CLEAR                                                     \
  drawables_off_readwrite.clear(std::memory_order_release)

  visual_list_t viewport_on = {};
  std::atomic_flag drawables_on_readwrite = ATOMIC_FLAG_INIT;
#define VIEWPORT_ON_SPIN                                                       \
  while (drawables_on_readwrite.test_and_set(std::memory_order_acquire))
#define VIEWPORT_ON_CLEAR                                                      \
  drawables_on_readwrite.clear(std::memory_order_release)

  bool surface_prime(void);
  void plot(context_cairo_region_t &plotArea);
  void flush(void);
  void device_offset(double x, double y);
  void device_scale(double x, double y);

  void resize_surface(const int w, const int h);

  void offsetPosition(const int x, const int y);
  void surface_brush(painter_brush_t &b);

  void render(void);
  void add_visual(std::shared_ptr<display_visual_t> _obj);
  void partition_visibility(void);
  void state(std::shared_ptr<display_visual_t> obj);
  void state(int x, int y, int w, int h);
  bool state(void);
  void state_surface(int x, int y, int w, int h);
  void state_notify_complete(void);

  void clear(void);

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(this)),
                 unit_memory_hash_code(), window_x, window_y, window_width,
                 window_height, window_open, brush.hash_code());

    return __value;
  }

public:
  short window_x = 0;
  short window_y = 0;
  unsigned short window_width = 0;
  unsigned short window_height = 0;
  bool window_open = false;

  std::atomic_flag lockBrush = ATOMIC_FLAG_INIT;
#define BRUSH_SPIN while (lockBrush.test_and_set(std::memory_order_acquire))
#define BRUSH_CLEAR lockBrush.clear(std::memory_order_release)
  painter_brush_t brush = painter_brush_t("white");

  cairo_t *cr = nullptr;

  cairo_rectangle_t viewport_rectangle = cairo_rectangle_t();

private:
  std::list<context_cairo_region_t> _regions = {};
  typedef std::list<context_cairo_region_t>::iterator region_iter_t;

  std::atomic_flag lockRegions = ATOMIC_FLAG_INIT;
#define REGIONS_SPIN while (lockRegions.test_and_set(std::memory_order_acquire))
#define REGIONS_CLEAR lockRegions.clear(std::memory_order_release)

  typedef struct _WH {
    int w = 0;
    int h = 0;
    _WH(int _w, int _h) : w(_w), h(_h) {}
  } __WH;
  std::list<_WH> _surfaceRequests = {};
  typedef std::list<_WH>::iterator surface_requests_iter_t;
  std::atomic_flag lockSurfaceRequests = ATOMIC_FLAG_INIT;
#define SURFACE_REQUESTS_SPIN                                                  \
  while (lockSurfaceRequests.test_and_set(std::memory_order_acquire))

#define SURFACE_REQUESTS_CLEAR                                                 \
  lockSurfaceRequests.clear(std::memory_order_release)

  int offsetx = 0, offsety = 0;
  void apply_surface_requests(void);
  std::mutex mutexRenderWork = {};
  std::condition_variable cvRenderWork = {};

public:
  // if render request time for objects are less than x ms
  int cache_threshold = 200;

  std::atomic<bool> clearing_frame = false;
  Display *xdisplay = nullptr;
  xcb_connection_t *connection = nullptr;
  xcb_screen_t *screen = nullptr;
  xcb_drawable_t window = 0;
  xcb_gcontext_t graphics = 0;

  xcb_visualtype_t *visualType = nullptr;

  // xcb -- keyboard
  xcb_key_symbols_t *syms = nullptr;

  cairo_surface_t *xcbSurface = nullptr;
  std::atomic_flag lockXCBSurface = ATOMIC_FLAG_INIT;
#define XCB_SPIN while (lockXCBSurface.test_and_set(std::memory_order_acquire))
#define XCB_CLEAR lockXCBSurface.clear(std::memory_order_release)
  void lock(bool b) {
    if (b) {
      XCB_SPIN;
    } else {
      XCB_CLEAR;
    }
  }
  bool preclear = false;
}; // namespace uxdevice
} // namespace uxdevice

namespace {
typedef std::function<void(display_context_t &context)> draw_logic_t;

}
