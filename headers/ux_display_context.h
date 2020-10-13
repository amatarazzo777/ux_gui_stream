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
\author Anthony Matarazzo
\file ux_display_context.h
\date 5/12/20
\version 1.0
 \details CLass holds the display window context, gui drawing, cairo
 context, and provides an interface for threads running to
 invalidate part of the surface, resize the surface. The
 unit_memory_t class is within the public members and holds
 the state of the last used data parameters.

*/
#pragma once

#include <ux_base.h>
#include <ux_hash.h>
#include <ux_error.h>
#include <ux_visitor_interface.h>
#include <ux_enums.h>

#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>
#include <ux_pipeline_memory.h>
#include <ux_display_visual.h>
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

/**
 * @internal
 * @class
 * @brief
 */
class display_context_t : virtual public hash_members_t,
                          virtual system_error_t,
                          public pipeline_memory_t {
public:
  display_context_t(void) {}

  display_context_t(const display_context_t &other)
    : hash_members_t(other), system_error_t(other), pipeline_memory_t(other),
      window_x(other.window_x), window_y(other.window_y),
      window_width(other.window_width), window_height(other.window_height),
      window_open(other.window_open), cr(cairo_reference(other.cr)),
      xdisplay(other.xdisplay), connection(other.connection),
      screen(other.screen), window(other.window), graphics(other.graphics),
      visualType(other.visualType), syms(other.syms),
      xcbSurface(other.xcbSurface), _regions(other._regions),
      _surfaceRequests(other._surfaceRequests) {}

  // move constructor
  display_context_t(display_context_t &&other) noexcept
    : hash_members_t(other), system_error_t(other), pipeline_memory_t(other),
      window_x(other.window_x), window_y(other.window_y),
      window_width(other.window_width), window_height(other.window_height),
      window_open(other.window_open), cr(other.cr), xdisplay(other.xdisplay),
      connection(other.connection), screen(other.screen), window(other.window),
      graphics(other.graphics), visualType(other.visualType), syms(other.syms),
      xcbSurface(other.xcbSurface), _regions(other._regions),
      _surfaceRequests(other._surfaceRequests) {}

  display_context_t &operator=(const display_context_t &other) {
    hash_members_t::operator=(other);
    system_error_t::operator=(other);
    pipeline_memory_t::operator=(other);
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
  virtual void pipeline_acquire(){};
  virtual bool pipeline_has_required_linkages(void) { return true; }
  virtual std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(this)),
                 pipeline_memory_hash_code(), window_x, window_y, window_width,
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

  cairo_t *cr = {};

  cairo_rectangle_t viewport_rectangle = cairo_rectangle_t();

  // if render request time for objects are less than x ms
  int cache_threshold = 200;

  std::atomic<bool> clearing_frame = false;
  Display *xdisplay = {};
  xcb_connection_t *connection = {};
  xcb_screen_t *screen = {};
  xcb_drawable_t window = {};
  xcb_gcontext_t graphics = {};
  xcb_visualtype_t *visualType = {};
  xcb_key_symbols_t *syms = {};

  cairo_surface_t *xcbSurface = {};
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

  int offsetx = {}, offsety = {};
  void apply_surface_requests(void);
  std::mutex mutexRenderWork = {};
  std::condition_variable cvRenderWork = {};

}; // namespace uxdevice
} // namespace uxdevice
