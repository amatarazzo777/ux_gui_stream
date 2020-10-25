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
 * @file ux_drawing_unit_primitives.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <base/unit_object.h>
#include <api/path/curve_storage.h>

// clang-format on

uxdevice::curve_storage_t::curve_storage_t() {}

uxdevice::curve_storage_t::curve_storage_t(double _x1, double _y1, double _x2,
                                           double _y2, double _x3, double _y3)
    : x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3) {}

uxdevice::curve_storage_t::~curve_storage_t() {}

std::size_t uxdevice::curve_storage_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(curve_storage_t)), x1, y1, x2,
               y2, x3, y3);
  return __value;
}
