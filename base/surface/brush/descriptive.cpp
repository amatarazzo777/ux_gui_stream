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
 * @file descriptive.cpp
 * @date 5/12/20
 * @version 1.0
 * @details Class provides the painting object interface which invokes the
 * appropriate cairo API. The notable virtual method is the emit function which
 * applies the cairo text_color_t setting.
 */
// clang-format off

#include <base/unit_object.h>
#include "painter_brush.h"

// clang-format on

/// @brief default constructor
uxdevice::painter_brush_t::descriptive_definition_t::
    descriptive_definition_t() {}

/// @brief string
uxdevice::painter_brush_t::descriptive_definition_t::descriptive_definition_t(
    const std::string &_description)
    : paint_definition_base_t(_description) {}

/// @brief copy constructor
uxdevice::painter_brush_t::descriptive_definition_t::descriptive_definition_t(
    const descriptive_definition_t &other)
    : paint_definition_base_t(other) {}

/// @brief move  constructor
uxdevice::painter_brush_t::descriptive_definition_t::descriptive_definition_t(
    descriptive_definition_t &&other) noexcept
    : paint_definition_base_t(other) {}

/// @brief de-constructor
uxdevice::painter_brush_t::descriptive_definition_t::
    ~descriptive_definition_t() {}

/**
 * @fn uxdevice::painter_brush_t::descriptive_definition_t operator =&(const
 * descriptive_definition_t&)
 * @brief copy assignment
 *
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::descriptive_definition_t &
uxdevice::painter_brush_t::descriptive_definition_t::operator=(
    const descriptive_definition_t &other) {
  paint_definition_base_t::operator=(other);
  return *this;
}

/**
 * @fn uxdevice::painter_brush_t::descriptive_definition_t operator
 * =&(descriptive_definition_t&&)
 * @brief move assignment
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::descriptive_definition_t &
uxdevice::painter_brush_t::descriptive_definition_t::operator=(
    descriptive_definition_t &&other) noexcept {
  paint_definition_base_t::operator=(other);
  return *this;
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::painter_brush_t::descriptive_definition_t::hash_code(
    void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(painter_brush_t)),
               paint_definition_base_t::hash_code());

  return __value;
}
