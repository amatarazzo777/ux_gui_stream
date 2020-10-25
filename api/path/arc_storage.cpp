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
 * @file arc_storage.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <base/unit_object.h>
#include <api/path/arc_storage.h>

// clang-format on

uxdevice::arc_storage_t::arc_storage_t::arc_storage_t() {}

uxdevice::arc_storage_t::arc_storage_t::arc_storage_t(double _xc, double _yc,
                                                      double _radius,
                                                      double _angle1,
                                                      double _angle2)
    : xc(_xc), yc(_yc), radius(_radius), angle1(_angle1), angle2(_angle2) {}
virtual uxdevice::arc_storage_t::~arc_storage_t() {}

std::size_t uxdevice::arc_storage_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(arc_storage_t)), xc, yc, radius,
               angle1, angle2);
  return __value;
}
