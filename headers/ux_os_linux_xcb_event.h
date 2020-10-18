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
 * @author Anthony Matarazzo
 * @file ux_os_linux_xcb_event.hpp
 * @date 9/7/20
 * @version 1.0
 *  @details  event class
 */

namespace uxdevice {
class window_manager_t;

/**
 * @class keyboard_device_event_t
 * @brief holds and processes keyboard messages.
 *
 */
class keyboard_device_event_t
  : public keyboard_device_event_base_t<xcb_generic_event_t *,
                                        xcb_key_press_event_t *,
                                        xcb_key_release_event_t *> {
public:
  using keyboard_device_event_base_t::keyboard_device_event_base_t;
  keyboard_device_event_t() = delete;
  keyboard_device_event_t(std::type_index ti);
  ~keyboard_device_event_t();
  void initialize();

  /// @brief copy assignment operator
  keyboard_device_event_t &operator=(const keyboard_device_event_t &other) {
    return *this;
  }

  /// @brief move assignment
  keyboard_device_event_t &operator=(keyboard_device_event_t &&other) noexcept {
    return *this;
  }

  /// @brief move constructor
  keyboard_device_event_t(keyboard_device_event_t &&other) noexcept
    : keyboard_device_event_base_t(other) {}

  /// @brief copy constructor
  keyboard_device_event_t(const keyboard_device_event_t &other)
    : keyboard_device_event_base_t(other) {}

  /**@brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.*/
  event_t get(void);

private:
  xcb_key_symbols_t *syms = {};
  window_manager_t *window_manager = {};
};

/**
 * @class mouse_button_evt_t
 * @brief
 *
 */
class mouse_device_event_t
  : public mouse_device_event_base_t<
      xcb_generic_event_t *, xcb_button_press_event_t *,
      xcb_button_release_event_t *, xcb_motion_notify_event_t *> {
  using mouse_device_event_base_t::mouse_device_event_base_t;

  ~mouse_device_event_t() {}

  /**@brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.*/
  event_t get(void);
  window_manager_t *window_manager = {};

}; // namespace uxdevice

/**
 * @class window_service_event_t
 * @brief a configuration notification message from xcb is very flexible.
 * The implementation provides a visitor pattern to generate more specific
 * message in conditions to ones that may be handled internally such as
 * resizing, moving or other effects that have a predisposed effect on the
 * system.
 *
 */
class window_service_event_t
  : window_service_event_base_t<xcb_generic_event_t *, xcb_expose_event_t *,
                                xcb_configure_notify_event_t *,
                                xcb_client_message_event_t *> {
public:
  using window_service_event_base_t::window_service_event_base_t;

  virtual ~window_service_event_t() {}

  /**@brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.*/
  event_t get(void);

  window_manager_t *window_manager = {};
};

} // namespace uxdevice
