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

/**
 *
 * \typedef visual_list_t
 * \brief used by the system to hold a list of items that draw.
 *
 */
namespace uxdevice {

/**
 * @brief forward declarations
 */

class hash_members_t;
class system_error_t;
class pipeline_memory_t;

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
  display_context_t(std::shared_ptr<os_window_manager> _wm);

  // copy constructor
  display_context_t(const display_context_t &other);
  // move constructor
  display_context_t(display_context_t &&other) noexcept;
  // copy assignment
  display_context_t &operator=(const display_context_t &other);

  bool surface_prime(void);
  void plot(context_cairo_region_t &plotArea);
  void flush(void);
  void device_offset(double x, double y);
  void device_scale(double x, double y);

  void resize_surface(const int w, const int h);

  void offset_position(const int x, const int y);
  void surface_brush(painter_brush_t &b);

  void render(void);
  void add_visual(std::shared_ptr<display_visual_t> obj);
  void partition_visibility(void);
  void state(std::shared_ptr<display_visual_t> obj);
  void state(int x, int y, int w, int h);
  bool state(void);
  void state_surface(int x, int y, int w, int h);
  void state_notify_complete(void);

  void clear(void);
  virtual void pipeline_acquire(){};
  virtual bool pipeline_has_required_linkages(void);
  virtual std::size_t hash_code(void) const noexcept;


public:
  std::shared_ptr<os_window_manager> window_manager = {};

  cairo_rectangle_t viewport_rectangle = cairo_rectangle_t();
  int offsetx = {}, offsety = {};

  // if render request time for objects are less than x ms
  int cache_threshold = 2000;

  std::atomic<bool> clearing_frame = false;

  display_visual_list_t viewport_off = {};
  std::mutex viewport_off_mutex = {};

  display_visual_list_t viewport_on = {};
  std::mutex viewport_on_mutex = {};

  std::mutex regions_storage_mutex = {};
  std::list<context_cairo_region_t> regions_storage = {};
  typedef std::list<context_cairo_region_t>::iterator region_iter_t;

  std::mutex render_work_mutex = {};
  std::condition_variable render_work_condition_variable = {};

  typedef struct _WH {
    int w = 0;
    int h = 0;
    _WH(int _w, int _h) : w(_w), h(_h) {}
  } __WH;
  std::list<_WH> surface_requests_storage = {};

  typedef std::list<_WH>::iterator surface_requests_iter_t;
  std::mutex surface_requests_storage_mutex = {};
};

} // namespace uxdevice
