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
 * @file tab_stops_storage.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <api/text/tab_stops_storage.h>
#include <base/unit_object.h>

// clang-format on

uxdevice::text_tab_stops_storage_t::text_tab_stops_storage_t() {}

uxdevice::text_tab_stops_storage_t::text_tab_stops_storage_t(
    const std::vector<double> &_value)
    : value(_value) {}

uxdevice::text_tab_stops_storage_t::~text_tab_stops_storage_t() {}

std::size_t uxdevice::text_tab_stops_storage_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(text_tab_stops_storage_t)));
  for (auto n : value)
    hash_combine(__value, n);
  return __value;
}
