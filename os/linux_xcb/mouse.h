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

/// @brief classes provide type for variant and wrap of pointer for ease of use.
class button_press_xcb_t
    : public ptr_type_class_alias<xcb_button_press_event_t *> {};

class button_release_xcb_t
    : public ptr_type_class_alias<xcb_button_release_event_t *> {};

class motion_notify_xcb_t
    : public ptr_type_class_alias<xcb_motion_notify_event_t *> {};

/**
 * @typedef xcb_mouse_event_t
 * @brief
 *
 */
typedef device_event_base_t<xcb_generic_event_t *, std::monostate,
                            button_press_xcb_t, button_release_xcb_t,
                            motion_notify_xcb_t>
    mouse_event_xcb_t;

/**
 * @class xcb_mouse_device_t
 * @brief
 *
 */
class mouse_device_xcb_t : public mouse_device_base_t<mouse_event_xcb_t> {
public:
  using mouse_device_base_t::mouse_device_base_t;

  ~mouse_device_xcb_t() {}

  /// @brief copy assignment operator
  mouse_device_xcb_t &operator=(const mouse_device_xcb_t &other) {
    mouse_device_base_t::operator=(other);
    return *this;
  }

  /// @brief move assignment
  mouse_device_xcb_t &operator=(mouse_device_xcb_t &&other) noexcept {
    mouse_device_base_t::operator=(other);
    return *this;
  }

  /// @brief move constructor
  mouse_device_xcb_t(mouse_device_xcb_t &&other) noexcept
      : mouse_device_base_t(other) {}

  /// @brief copy constructor
  mouse_device_xcb_t(const mouse_device_xcb_t &other)
      : mouse_device_base_t(other) {}

  /**
   * @internal
   * @fn mouse_device_xcb_t get*(void)
   * @brief must specialize this for interface.
   * interface abstract returns the visitor map. see the
   * ux_os_linux_event.cpp file for details.
   *
   * @return
   */
  mouse_device_xcb_t *get(void);

  std::shared_ptr<os_xcb_linux_t> window_manager = {};
};

using os_mouse_device_t = mouse_device_xcb_t;

} // namespace uxdevice
