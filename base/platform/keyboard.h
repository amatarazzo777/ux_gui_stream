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
@file ux_os_window_manager_event_base.h
@date 10/15/20
@version 1.0
@brief header file holds the template event base for devices that respond to
message queue filters.
*/

namespace uxdevice {

/**
 * @class keyboard_device_event_t
 * @brief holds and processes keyboard messages.
 *
 */
template <typename T> class keyboard_device_base_t : public T {
public:
  using T::T;
  keyboard_device_base_t() : T() {}
  // typedef typename T::data_storage_t data_storage_t;
  // keyboard_device_base_t(data_storage_t msg) : T(msg) {}
  virtual ~keyboard_device_base_t() {}

  /// @brief copy assignment operator
  keyboard_device_base_t &operator=(const keyboard_device_base_t &other) {
    T::operator=(other);
    sym = other.sym;
    c = other.c;
    return *this;
  }

  /// @brief move assignment
  keyboard_device_base_t &operator=(keyboard_device_base_t &&other) noexcept {
    T::operator=(other);
    sym = other.sym;
    c = other.c;
    return *this;
  }

  /// @brief move constructor
  keyboard_device_base_t(keyboard_device_base_t &&other) noexcept
      : T(other), sym(other.sym), c(other.c) {}

  /// @brief copy constructor
  keyboard_device_base_t(const keyboard_device_base_t &other)
      : T(other), sym(other.sym), c(other.c) {}

  /** @brief data interface */
  uint32_t sym = {};
  std::array<char, 16> c = {};
};
} // namespace uxdevice
