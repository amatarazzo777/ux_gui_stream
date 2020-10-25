#pragma once

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
@author Anthony Matarazzo
@file ux_os_window_manager_event_base.h
@date 10/15/20
@version 1.0
@brief header file holds the template event base for devices that respond to
message queue filters.
*/

namespace uxdevice {

/**
 * @class device_event_base_t
 * @brief handles the xcb event memory with a free. This does not keep any
 * reference counting.
 * @tparam T  The window base message type.
 * @tparam Args... The variant storage for the device.
 *
 */
template <typename T, typename... Args> class device_event_base_t {
public:
  /**
   * @internal
   * @typedef generalized_msg_t
   * @brief the os message pointer format (linux xcb) or tuple, struct, etc.
   */
  typedef T generalized_msg_t;

  /**
   * @internal
   * @typedef data_storage_t
   * @brief The message forms accepted by the device.
   */
  typedef std::variant<Args...> data_storage_t;

  /**
   * @internal
   * @fn  device_event_base_t()
   * @brief
   *
   */
  device_event_base_t() {}

  /**
   * @fn  device_event_base_t(data_storage_t)
   * @brief
   *
   * @param _msg
   */
  device_event_base_t(data_storage_t _msg) : data(_msg) {}

  /**
   * @fn  ~device_event_base_t()
   * @brief
   *
   */
  virtual ~device_event_base_t() {}

  /// @brief copy assignment operator
  device_event_base_t &operator=(const device_event_base_t &other) {
    return *this;
  }

  /// @brief move assignment
  device_event_base_t &operator=(device_event_base_t &&other) noexcept {
    return *this;
  }

  /// @brief move constructor
  device_event_base_t(device_event_base_t &&other) noexcept {}

  /// @brief copy constructor
  device_event_base_t(const device_event_base_t &other) {}

  /**
   * @internal
   * @overload
   * @fn T get(void)
   * @brief returns the associated OS data stored within the variant. While the
   * template parameter is not explicitly a pointer, here it is reference as a
   * pointer. This method simply reduces the syntax to a default. the pointer
   * can be used with os apis or inspected more carefully.
   *
   * @tparam T
   * @return pointer to defined type
   */
  template <typename T2> T2 get(void) { return std::get<T2>(data); }

  /**
   * @internal
   * @var alias
   * @brief The alias is a signature that provides meaning to the information
   * that is specific to how the system will interpret it. These are names
   * within the API such as listen_mousemove_t. The data is kept as unit along
   * with the alias.*/
  std::type_index alias = std::type_index(typeid(this));

  /**
   * @internal
   * @var data
   * @brief holds each of the message types to be processed as a visitor. */
  data_storage_t data = {};

  /**
   * @internal
   * @var bvideo_output
   * @brief informs if event requires direct painting. such as expose. */
  bool bvideo_output = {};
};

} // namespace uxdevice
