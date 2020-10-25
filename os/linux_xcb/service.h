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

class configure_notify_xcb_t
    : public ptr_type_class_alias<xcb_configure_notify_event_t *> {};

class client_message_xcb_t
    : public ptr_type_class_alias<xcb_client_message_event_t *> {};

/// @brief the keyboard event types
using keyboard_event_xcb_t =
    device_event_base_t<xcb_generic_event_t *, std::monostate, key_press_xcb_t,
                        key_release_xcb_t>;

/**
 * @typedef xcb_window_service_event_t
 * @brief
 */
typedef device_event_base_t<xcb_generic_event_t *, std::monostate,
                            surface_expose_xcb_t, configure_notify_xcb_t,
                            client_message_xcb_t>
    window_service_event_xcb_t;

/**
 * @class window_service_event_t
 * @brief a configuration notification message from xcb is very flexible.
 * The implementation provides a visitor pattern to generate more specific
 * message in conditions to ones that may be handled internally such as
 * resizing, moving or other effects that have a predisposed effect on the
 * system.
 *
 */
class window_service_xcb_t : window_service_base_t<window_service_event_xcb_t> {
public:
  using window_service_base_t::window_service_base_t;
  using data_storage_t = typename window_service_event_xcb_t::data_storage_t;
  using generalized_msg_t =
      typename window_service_event_xcb_t::generalized_msg_t;

  virtual ~window_service_xcb_t() {}
  /// @brief copy assignment operator
  window_service_xcb_t &operator=(const window_service_xcb_t &other) {
    window_service_base_t::operator=(other);
    return *this;
  }

  /// @brief move assignment
  window_service_xcb_t &operator=(window_service_xcb_t &&other) noexcept {
    window_service_base_t::operator=(other);
    return *this;
  }

  /// @brief move constructor
  window_service_xcb_t(window_service_xcb_t &&other) noexcept
      : window_service_base_t(other) {}

  /// @brief copy constructor
  window_service_xcb_t(const window_service_xcb_t &other)
      : window_service_base_t(other) {}

  /**@brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.*/
  window_service_xcb_t *get(void);

  std::shared_ptr<os_xcb_linux_t> window_manager = {};
};

using os_window_service_t = window_service_xcb_t;
