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
 * @file ux_os_xcb_linux.h
 * @date Oct 12, 2020
 * @version 1.0
 * @details The file implements the os interface which provides the
 * communication to the os for basic visualization services such as window
 * manager functions. THe message queue processor is defined and implemented
 * here..
 */

#pragma once

namespace uxdevice {

/**
 * @internal
 * @class os_xcb_linux_t
 * @brief defines the window manager for xcb linux for cairo invocation and
 * surface building.
 *
 * The template parameter to the window_manager_base_t provides event queue
 * processing along with the instantiation of an abstract template function that
 * must be specialized with the event_t get function. The ability to provide a
 * platform implementation using this template is very compatible with other
 * systems. A structure * to os defined type would include other attributes of
 * the object within a domain such as hwnd, style, border, transparency, title
 * bar options, pop up window, child window are attributes.
 *
 * By inheriting from the window_manager_base_t, the following functions and
 * data members are provided in the base:
 *  void draw_fn(const std::function<void(cairo_t *)> &fn);
  void surface_fn(const std::function<void(cairo_surface_t *)> &fn);
  void video_flush(void);
 *  template <typename DEVICE, typename CAST>
  void dispatch(GENERIC_OS_MSG_T msg);

    short window_x = 0;
  short window_y = 0;
  unsigned short window_width = 0;
  unsigned short window_height = 0;
  bool window_open = false;

 */
class os_xcb_linux_t : public window_manager_base_t {
public:
  os_xcb_linux_t() {}
  virtual ~os_xcb_linux_t() {}

  /// @brief copy constructor
  os_xcb_linux_t(const os_xcb_linux_t &other) {}

  /// @brief move constructor
  os_xcb_linux_t(os_xcb_linux_t &&other) noexcept {}

  /// @brief copy assignment operator
  os_xcb_linux_t &operator=(const os_xcb_linux_t &other);

  /// @brief move assignment
  os_xcb_linux_t &operator=(os_xcb_linux_t &&other) noexcept;

  void open_window(const coordinate_list_t &coord,
                   const std::string &sWindowTitle,
                   const event_handler_t &dispatch_events);
  void close_window(void);
  void flush_window(void);

  /** @internal
   * @var message_translator
   * @brief system domain translator. must be filled by inheriting
   */
  static const message_dispatch_t message_dispatch;

private:
  void message_loop(void);

  std::mutex xcb_event_queue_mutex = {};
  std::list<xcb_generic_event *> xcb_event_queue = {};
  std::mutex xcb_event_queue_condition_mutex = {};
  std::condition_variable xcb_event_queue_condition_variable = {};

  std::atomic<bool> bProcessing = false;
  Display *xdisplay = {};
  xcb_connection_t *connection = {};
  xcb_screen_t *screen = {};
  xcb_drawable_t window = {};
  xcb_gcontext_t graphics = {};
  xcb_visualtype_t *visual_type = {};

};

} // namespace uxdevice
