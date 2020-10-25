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
@file key_storage.h
@date 5/12/20
@version 1.0
@details

*/

namespace uxdevice {
class indirect_index_storage_t;

/**
}
@internal
@class key_storage_t
@brief
*/
class key_storage_t {
public:
  key_storage_t() {}
  virtual ~key_storage_t() {}
  /// @brief copy constructor
  key_storage_t(const indirect_index_storage_t &_key);

  /// @brief move constructor
  key_storage_t(key_storage_t &&other) noexcept;

  /// @brief copy assignment operator
  key_storage_t &operator=(const key_storage_t &other);

  /// @brief move assignment
  key_storage_t &operator=(key_storage_t &&other) noexcept;

  /// @brief move constructor
  key_storage_t(key_storage_t &&other) noexcept;

  indirect_index_storage_t key = {};
};
} // namespace uxdevice
