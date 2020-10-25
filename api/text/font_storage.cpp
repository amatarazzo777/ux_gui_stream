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
 * @file font_storage.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <api/text/font_storage.h>
#include <base/unit_object.h>

// clang-format on

uxdevice::text_font_storage_t::text_font_storage_t()
    : description{}, font_ptr(nullptr) {}

uxdevice::text_font_storage_t::text_font_storage_t(
    const std::string &_description)
    : description(_description), font_ptr(nullptr) {}

/// @brief move constructor
uxdevice::text_font_storage_t::text_font_storage_t(
    text_font_storage_t &&other) noexcept
    : description(other.description), font_ptr(other.font_ptr) {}

/// @brief copy constructor
uxdevice::text_font_storage_t::text_font_storage_t(
    const text_font_storage_t &other)
    : description(other.description), font_ptr(nullptr) {}

uxdevice::text_font_storage_t::~text_font_storage_t() {
  if (font_ptr)
    pango_font_description_free(font_ptr);
}

uxdevice::text_font_storage_t &uxdevice::text_font_storage_t::operator=(
    const text_font_storage_t &&other) noexcept {
  description = other.description;
  return *this;
}

uxdevice::text_font_storage_t &
uxdevice::text_font_storage_t::operator=(const text_font_storage_t &other) {
  description = other.description;
  return *this;
}

uxdevice::text_font_storage_t &
uxdevice::text_font_storage_t::operator=(const std::string &_desc) {
  description = _desc;
  return *this;
}

/// @brief move assignment
uxdevice::text_font_storage_t &
uxdevice::text_font_storage_t::operator=(const std::string &&_desc) noexcept {
  description = _desc;
  return *this;
}

std::size_t uxdevice::text_font_storage_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(text_font_storage_t)),
               description);

  return __value;
}
