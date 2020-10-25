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
/**
 * @class xcb_ptr_wrapper
 * @brief the class is used to hold aliased pointer types from xcb namespace.
 * The reason this is needed is because the std::variant template expects that
 * all items be unique. However the xcb_event_ptrs often use the same structure
 * but call it a type using the typedef. The creates a problem where the
 * std::variant thinks that all xcb_ptr types are of the same type. So by using
 * the template class to minimize storage and move, assignment constructors, the
 * xcb pointer types are held in a unique class type.
 *
 * @tparam T
 */
template <typename T> class ptr_type_class_alias {
public:
  ptr_type_class_alias() {}
  ptr_type_class_alias(xcb_generic_event_t *_msg) : ptr((T)_msg) {}
  ptr_type_class_alias &operator=(const ptr_type_class_alias &other) {
    return *this;
  }

  T operator->() { return ptr; }
  T get(void) { return ptr; }

  /// @brief move assignment
  ptr_type_class_alias &operator=(ptr_type_class_alias &&other) noexcept {
    ptr = other.ptr;
    return *this;
  }

  /// @brief move constructor
  ptr_type_class_alias(ptr_type_class_alias &&other) noexcept
      : ptr(other.ptr) {}

  /// @brief copy constructor
  ptr_type_class_alias(const ptr_type_class_alias &other) { ptr = other.ptr; }
  T ptr = {};
};
} // namespace uxdevice
