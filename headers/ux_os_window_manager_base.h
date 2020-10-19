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
#pragma once

/**
@author Anthony Matarazzo
@file ux_os_window_manager_base.h
@date 10/15/20
@version 1.0
@brief
*/

namespace uxdevice {
class window_manager_base_t;
class painter_brush_t;

/**
 * @typedef coordinate_list_t
 * @brief An std::list used to communicate coordinate for the window. varying
 * pairs may be given. two or four.
 */
typedef std::list<short int> coordinate_list_t;

/** @internal
 * @typedef
 * @brief system domain translator.
 */
template <typename... Args>
using message_dispatch_t =
  std::unordered_map<std::size_t,
                     std::function<void(window_manager_base_t *, ...)>>;

/**
 * @class window_manager_abstract_t
 * @brief class that all target os window manager must inherit.
 *
 */

class window_manager_base_t : virtual public system_error_t {
public:
  window_manager_base_t() {}
  virtual ~window_manager_base_t() {}

  /// @brief copy assignment operator
  window_manager_base_t &operator=(const window_manager_base_t &other) {
    return *this;
  }

  /// @brief move assignment
  window_manager_base_t &operator=(window_manager_base_t &&other) noexcept {
    return *this;
  }

  /// @brief move constructor
  window_manager_base_t(window_manager_base_t &&other) noexcept {}

  /// @brief copy constructor
  window_manager_base_t(const window_manager_base_t &other) {}

  /**
   * @fn void open_window(const coordinate_list_t&, const std::string&, const
   * painter_brush_t&, const event_handler_t&)=0
   * @brief
   *
   * @param coord
   * @param sWindowTitle
   * @param background
   * @param dispatch_events
   */
  virtual void open_window(const coordinate_list_t &coord,
                           const std::string &sWindowTitle,
                           const painter_brush_t &background,
                           const event_handler_t &dispatch_events) = 0;
  /**
   * @fn void close_window(void)=0
   * @brief
   *
   */
  virtual void close_window(void) = 0;

  /**
   * @fn void flush_window(void)=0
   * @brief
   *
   */
  virtual void flush_window(void) = 0;

  // these functions handled in base
  void draw_fn(const std::function<void(cairo_t *)> &fn);
  void surface_fn(const std::function<void(cairo_surface_t *)> &fn);
  void video_flush(void);

  /**
   * @fn void event(generic_os_event_queue_message_t_*)
   * @brief dispatches the event using the generalized event handler for a
   * device. such as mouse, keyboard or system. The objects internally create an
   * event_t object that the system interprets which is generic.
   *
   *The
   * @tparam DEVICE
   * @tparam CAST
   * @param msg
   */
  template <typename DEVICE, typename CAST> void dispatch(std::any msg) {
    // first template parameter must be a base of event_base_t
    // which is an object that handles creation/transform
    // static_assert(std::is_base_of<event_base_t, DEVICE>::value);

    // this creates the
    auto o = DEVICE(msg);
    auto evt = o.get();
    dispatch_event(evt);
  }

  /** @internal
   * @brief these variables are common across instances yet they are managed by
   * the inheriting object as part of base OS. However the cairo_t represents
   * across platforms so it can be used like this. Minimizes and simplifies
   * code.
   */
  std::mutex surface_mutex = {};
  cairo_surface_t *surface = {};
  std::mutex cr_mutex = {};
  cairo_t *cr = {};
  std::mutex background_brush_mutex = {};
  painter_brush_t background_brush = painter_brush_t("white");

  /** @internal
   * @brief these variables are common across instances yet they are managed by
   * the inheriting object as part of base OS.
   */
  short window_x = 0;
  short window_y = 0;
  unsigned short window_width = 0;
  unsigned short window_height = 0;
  bool window_open = false;

  std::bitset<64> window_style = {};
};

} // namespace uxdevice
