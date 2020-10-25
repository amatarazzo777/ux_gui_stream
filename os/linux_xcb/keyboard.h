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

// @brief pointer wrappers
class key_press_xcb_t : public ptr_type_class_alias<xcb_key_press_event_t *> {};

class key_release_xcb_t
    : public ptr_type_class_alias<xcb_key_release_event_t *> {};

/**
 * @typedef xcb_mouse_event_t
 * @brief
 *
 */
typedef device_event_base_t<xcb_generic_event_t *, std::monostate,
                            key_press_xcb_t, key_release_xcb_t>
    keyboard_event_xcb_t;

/**
 * @class xcb_keyboard_device_t
 * @brief holds and processes keyboard messages.
 *
 */
class keyboard_device_xcb_t
    : public keyboard_device_base_t<keyboard_event_xcb_t> {
public:
  using keyboard_device_base_t::keyboard_device_base_t;
  using data_storage_t = typename keyboard_event_xcb_t::data_storage_t;
  using generalized_msg_t = typename keyboard_event_xcb_t::generalized_msg_t;

  keyboard_device_xcb_t();
  keyboard_device_xcb_t(typename keyboard_event_xcb_t::data_storage_t _msg)
      : keyboard_device_base_t(_msg) {}

  ~keyboard_device_xcb_t();
  void initialize();

  /// @brief copy assignment operator
  keyboard_device_xcb_t &operator=(const keyboard_device_xcb_t &other) {
    keyboard_device_base_t::operator=(other);
    return *this;
  }

  /// @brief move assignment
  keyboard_device_xcb_t &operator=(keyboard_device_xcb_t &&other) noexcept {
    keyboard_device_base_t::operator=(other);
    return *this;
  }

  /// @brief move constructor
  keyboard_device_xcb_t(keyboard_device_xcb_t &&other) noexcept
      : keyboard_device_base_t(other) {}

  /// @brief copy constructor
  keyboard_device_xcb_t(const keyboard_device_xcb_t &other)
      : keyboard_device_base_t(other) {}

  /**@brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.*/
  keyboard_device_xcb_t *get(void);

private:
  xcb_key_symbols_t *syms = {};
  std::shared_ptr<os_xcb_linux_t> window_manager = {};
};

using os_keyboard_device_t = keyboard_device_xcb_t;

} // namespace uxdevice
