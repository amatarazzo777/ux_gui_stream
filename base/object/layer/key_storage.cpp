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
 * @author Anthony Matarazzo
 * @file key_storage.cpp
 * @date 9/19/20
 * @version 1.0
 * @brief
 */
#include <base/inc/std_base.h>
#include <base/inc/object/layer/indirect_index.h>
#include <base/inc/object/layer/key_storage.h>

uxdevice::key_storage_t::key_storage_t() {}

uxdevice::key_storage_t::~key_storage_t() {}

uxdevice::key_storage_t(const indirect_index_storage_t &_key) : key(_key) {}

/// @brief move constructor
uxdevice::key_storage_t::key_storage_t(key_storage_t &&other) noexcept {
  key = std::move(other.key);
}

/// @brief copy constructor
uxdevice::key_storage_t::key_storage_t(const key_storage_t &other) {
  *this = other;
}

/// @brief copy assignment operator
uxdevice::key_storage_t &
uxdevice::key_storage_t::operator=(const key_storage_t &other) {
  key = other.key;
  return *this;
}
/// @brief move assignment
uxdevice::key_storage_t::key_storage_t &
operator=(key_storage_t &&other) noexcept {
  key = std::move(other.key);
  return *this;
}
