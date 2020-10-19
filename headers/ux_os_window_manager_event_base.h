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
class event_base_t {
public:
  std::type_index alias = std::type_index(typeid(this));
  std::any i;
};

/**
 * @class evt_base_t
 * @brief handles the xcb event memory with a free. This does not keep any
 * reference counting.
 * @tparam OS_MSG  The window base message type. This can be a pointer, which is
 * the best candidate for it perhaps. However a direct struct could also be
 * implemented such as window {hwnd, wp, lparam, user_data}. The template below
 * may need to change a little. The data cast of the message (T_MSG)(msg) in the
 * constructor parameters might need to change.
 * @tparam Args... The variant storage for the device.
 *
 */
template <typename OS_MSG, typename... Args> class device_event_base_t {
public:
  // template constructor accepts generic os event. The .
  template <typename T_MSG>
  device_event_base_t(OS_MSG _msg) : data((T_MSG)(_msg)), msg(_msg) {}

  device_event_base_t(std::any _msg) : msg(std::any_cast<OS_MSG>(_msg)) {}

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
  template <typename T> T get(void) { return std::get<T>(data); }

  /**
   * @internal
   * @overload
   * @fn T get(void)
   * @brief The get override for the event_t object type. When this object type
   * is built, the data from the os layer is interpreted and the object can be
   * used to manufacture an event that is designed for the system. The visit map
   * within the ux_os_linux_xcb_event.cpp file performs this operation. After
   * completion, the objects data members have the values in summary from the
   * xcb event.
   *
   * @return
   */
  virtual device_event_base_t<OS_MSG, Args...> *get(void) = 0;
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
  std::variant<Args...> data = {};
  /**
   * @internal
   * @var msg
   * @brief The generic os message.*/
  OS_MSG msg = {};
  /**
   * @internal
   * @var bvideo_output
   * @brief informs if event requires direct painting. such as expose. */
  bool bvideo_output = {};
};

/**
 * @class keyboard_device_event_t
 * @brief holds and processes keyboard messages.
 *
 */
template <typename T> class keyboard_device_base_t : public T {
public:
  using T::T;
  keyboard_device_base_t(const std::type_index &_alias);
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

/**
 * @class mouse_button_evt_t
 * @brief
 *
 */
template <typename T> class mouse_device_base_t : public T {
public:
  using T::T;
  virtual ~mouse_device_base_t() {}
  /// @brief copy assignment operator
  mouse_device_base_t &operator=(const mouse_device_base_t &other) {
    T::operator=(other);
    x = other.x;
    y = other.y;
    d = other.d;
    return *this;
  }

  /// @brief move assignment
  mouse_device_base_t &operator=(mouse_device_base_t &&other) noexcept {
    T::operator=(other);
    x = other.x;
    y = other.y;
    d = other.d;
    return *this;
  }

  /// @brief move constructor
  mouse_device_base_t(mouse_device_base_t &&other) noexcept
    : T(other), x(other.x), y(other.y), d(other.d) {}

  /// @brief copy constructor
  mouse_device_base_t(const mouse_device_base_t &other)
    : T(other), x(other.x), y(other.y), d(other.d) {}

  short x = {};
  short y = {};
  short d = {};

}; // namespace uxdevice

/**
 * @class window_service_device_event_base_t
 * @brief a configuration notification message from xcb is very flexible.
 * The implementation provides a visitor pattern to generate more specific
 * message in conditions to ones that may be handled internally such as
 * resizing, moving or other effects that have a predisposed effect on the
 * system.
 *
 */
template <typename T> class window_service_base_t : public T {
public:
  using T::T;
  virtual ~window_service_base_t() {}
  /// @brief copy assignment operator
  window_service_base_t &operator=(const window_service_base_t &other) {
    T::operator=(other);
    x = other.x;
    y = other.y;
    w = other.w;
    h = other.h;
    return *this;
  }

  /// @brief move assignment
  window_service_base_t &operator=(window_service_base_t &&other) noexcept {
    T::operator=(other);
    x = other.x;
    y = other.y;
    w = other.w;
    h = other.h;
    return *this;
  }

  /// @brief move constructor
  window_service_base_t(window_service_base_t &&other) noexcept
    : T(other), x(other.x), y(other.y), w(other.w), h(other.h) {}

  /// @brief copy constructor
  window_service_base_t(const window_service_base_t &other)
    : T(other), x(other.x), y(other.y), w(other.w), h(other.h) {}

  short x = {};
  short y = {};
  short w = {};
  short h = {};
};

} // namespace uxdevice
