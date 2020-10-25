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
 @file ux_pipeline_memory.hpp
 @date 9/19/20
 @version 1.0
 @brief pipeline memory storage and visitation logic

 */

#pragma once

/**
 * @internal
 * @class unit_memory_storage_t
 * @brief class exposes template member functions in that class that it is
 * inherited in. The mechanism provides typed index storage of objects that may
 * be referenced at any time.
 */
namespace uxdevice {
typedef std::function<std::size_t(void)> hash_function_t;

/**
 * @internal
 * @class pipeline_memory_object_t
 *
 * @brief object storage
 */
class pipeline_memory_object_t {
public:
  pipeline_memory_object_t();

  pipeline_memory_object_t(std::any _o, std::size_t _bits,
                           accepted_interfaces_storage_t *_accepted_interfaces,
                           const hash_function_t _hash_function);
  /// @brief copy assignment operator
  pipeline_memory_object_t &operator=(const pipeline_memory_object_t &other) {
    return *this;
  }

  /// @brief move assignment
  pipeline_memory_object_t &
  operator=(pipeline_memory_object_t &&other) noexcept {
    return *this;
  }

  /// @brief move constructor
  pipeline_memory_object_t(pipeline_memory_object_t &&other) noexcept {}

  /// @brief copy constructor
  pipeline_memory_object_t(const pipeline_memory_object_t &other) {}

  /** @brief [] operator reduces syntax*/
  visitor_interface_t *operator[](std::type_index ti) noexcept {
    visitor_interface_t *ret = {};
    if (accept_interfaces) {
      auto n = accept_interfaces->find(ti);
      if (n != accept_interfaces->end())
        ret = n->second;
    }
    return ret;
  }
  std::any object = {};
  std::mutex data_mutex = {};
  std::size_t visitor_target_bits = {};
  accepted_interfaces_storage_t *accept_interfaces = {};
  hash_function_t hash_function = {};
};
