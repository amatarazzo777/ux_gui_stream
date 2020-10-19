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
class os_xcb_linux_t;

/**
 * @typedef xcb_keyboard_event_t
 * @brief accepted message pointer types. First is the generic input.
 */
typedef device_event_base_t<xcb_generic_event_t *,
                            xcb_key_press_event_t *, xcb_key_release_event_t *>
  xcb_keyboard_event_t;

/**
 * @class xcb_keyboard_device_t
 * @brief holds and processes keyboard messages.
 *
 */
class xcb_keyboard_device_t
  : public keyboard_device_base_t<xcb_keyboard_event_t> {
public:
  using keyboard_device_base_t::keyboard_device_base_t;
  xcb_keyboard_device_t() = delete;
  xcb_keyboard_device_t(std::type_index ti);
  ~xcb_keyboard_device_t();
  void initialize();

  /// @brief copy assignment operator
  xcb_keyboard_device_t &operator=(const xcb_keyboard_device_t &other) {
    keyboard_device_base_t::operator=(other);
    return *this;
  }

  /// @brief move assignment
  xcb_keyboard_device_t &operator=(xcb_keyboard_device_t &&other) noexcept {
    keyboard_device_base_t::operator=(other);
    return *this;
  }

  /// @brief move constructor
  xcb_keyboard_device_t(xcb_keyboard_device_t &&other) noexcept
    : keyboard_device_base_t(other) {}

  /// @brief copy constructor
  xcb_keyboard_device_t(const xcb_keyboard_device_t &other)
    : keyboard_device_base_t(other) {}

  /**@brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.*/
  xcb_keyboard_device_t *get(void);

private:
  xcb_key_symbols_t *syms = {};
  std::shared_ptr<os_xcb_linux_t> window_manager = {};
};

/**
 * @typedef xcb_mouse_event_t
 * @brief
 *
 */
typedef device_event_base_t<
  xcb_generic_event_t *,  xcb_button_press_event_t *,
  xcb_button_release_event_t *, xcb_motion_notify_event_t *>
  xcb_mouse_event_t;

/**
 * @class xcb_mouse_device_t
 * @brief
 *
 */
class xcb_mouse_device_t : public mouse_device_base_t<xcb_mouse_event_t> {
  using mouse_device_base_t::mouse_device_base_t;

  ~xcb_mouse_device_t() {}

  /// @brief copy assignment operator
  xcb_mouse_device_t &operator=(const xcb_mouse_device_t &other) {
    mouse_device_base_t::operator=(other);
    return *this;
  }

  /// @brief move assignment
  xcb_mouse_device_t &operator=(xcb_mouse_device_t &&other) noexcept {
    mouse_device_base_t::operator=(other);
    return *this;
  }

  /// @brief move constructor
  xcb_mouse_device_t(xcb_mouse_device_t &&other) noexcept
    : mouse_device_base_t(other) {}

  /// @brief copy constructor
  xcb_mouse_device_t(const xcb_mouse_device_t &other)
    : mouse_device_base_t(other) {}

  /**@brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.*/
  xcb_mouse_device_t *get(void);
  std::shared_ptr<os_xcb_linux_t> window_manager = {};
};

/**
 * @typedef xcb_window_service_event_t
 * @brief
 */
typedef device_event_base_t<
  xcb_generic_event_t *, xcb_button_press_event_t *,
  xcb_button_release_event_t *, xcb_motion_notify_event_t *>
  xcb_window_service_event_t;

/**
 * @class window_service_event_t
 * @brief a configuration notification message from xcb is very flexible.
 * The implementation provides a visitor pattern to generate more specific
 * message in conditions to ones that may be handled internally such as
 * resizing, moving or other effects that have a predisposed effect on the
 * system.
 *
 */
class xcb_window_service_t : window_service_base_t<xcb_window_service_event_t> {
public:
  using window_service_base_t::window_service_base_t;

  virtual ~xcb_window_service_t() {}
  /// @brief copy assignment operator
  xcb_window_service_t &operator=(const xcb_window_service_t &other) {
    window_service_base_t::operator=(other);
    return *this;
  }

  /// @brief move assignment
  xcb_window_service_t &operator=(xcb_window_service_t &&other) noexcept {
    window_service_base_t::operator=(other);
    return *this;
  }

  /// @brief move constructor
  xcb_window_service_t(xcb_window_service_t &&other) noexcept
    : window_service_base_t(other) {}

  /// @brief copy constructor
  xcb_window_service_t(const xcb_window_service_t &other)
    : window_service_base_t(other) {}

  /**@brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.*/
  xcb_window_service_t *get(void);

  std::shared_ptr<os_xcb_linux_t> window_manager = {};
};

} // namespace uxdevice
