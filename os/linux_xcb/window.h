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
/// @brief forward template class allows the os message and platform to be
/// resolved early within the build process.

/**
 * @typedef os_message_t
 * @brief this is used to generalize parts of the code here and else where. The
 * typedef should be defined.
 *
 */

typedef xcb_generic_event_t *os_message_t;


/**
 * @internal
 * @class os_xcb_linux_t
 * @brief The class provides the necessary implementation details according to
 * the window_manager_base_t class. The window_manager_t base class provides
 * functions that are used by the rest of the system. THe message queue and a
 * threaded dispatcher is provided by the base. The mutex operation of the
 * surface and drawing functions are handled. This class must implement the
 * function attach_surface().
 */

class os_xcb_linux_t : public window_manager_base_t<os_message_t> {
public:
  using window_manager_base_t::window_manager_base_t;

  os_xcb_linux_t();
  virtual ~os_xcb_linux_t();

  /// @brief move, assignment constructors, operators implementation
  os_xcb_linux_t(const os_xcb_linux_t &other);
  os_xcb_linux_t(os_xcb_linux_t &&other) noexcept;
  os_xcb_linux_t &operator=(const os_xcb_linux_t &other);
  os_xcb_linux_t &operator=(os_xcb_linux_t &&other) noexcept;

  /// @brief os specific implementation functions.
  void fn_open_window(void);
  void fn_close_window(void);
  void fn_flush_window(void);

  /// @brief the base window manager has a threaded event queue and a variant
  /// visit interface. These functions are called by this logic. The odd ball,
  /// fn_complete_message is a free(). This routine is called when the system
  /// completes processing of the message.
  xcb_generic_event_t *fn_wait_message(void);
  xcb_generic_event_t *fn_poll_message(void);
  void fn_complete_message(xcb_generic_event_t *);

  /// @brief the composition of the event and its logic causing invocation of
  /// event listeners attached.
  void fn_visit_dispatch(xcb_generic_event_t *e);

  /// @brief window attributes
  void fn_set_title(const std::string &s);

private:
  Display *xdisplay = {};
  xcb_connection_t *connection = {};
  xcb_screen_t *screen = {};
  xcb_drawable_t window = {};
  xcb_gcontext_t graphics = {};
  xcb_visualtype_t *visual_type = {};

  /// @brief wm_close message enable
  xcb_intern_atom_cookie_t cookie = {};
  xcb_intern_atom_reply_t *reply = {};
  xcb_intern_atom_cookie_t cookie2 = {};
  xcb_intern_atom_reply_t *reply2 = {};

  /// @brief these classes are friend because they use the private connection
  /// and xcb information to allocate or query resources.
  friend class keyboard_device_xcb_t;
  friend class mouse_device_xcb_t;
  friend class window_service_xcb_t;
};

/// @brief type alias to generalize other parts of the code if necessary.
using os_window_manager = os_xcb_linux_t;

} // namespace uxdevice
