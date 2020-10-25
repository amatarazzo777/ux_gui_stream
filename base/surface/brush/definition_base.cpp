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
 * @file definition_base.cpp
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

/**
 * @fn  paint_definition_base_t()
 * @brief
 *
 */
uxdevice::painter_brush_t::paint_definition_base_t::paint_definition_base_t() {}
/**
 * @fn  paint_definition_base_t(const std::string&)
 * @brief
 *
 * @param _description
 */
uxdevice::painter_brush_t::paint_definition_base_t::paint_definition_base_t(
    const std::string &_description)
    : description(_description) {}
/**
 * @fn  ~paint_definition_base_t()
 * @brief
 *
 */
uxdevice::painter_brush_t::paint_definition_base_t::~paint_definition_base_t() {
}

/// @brief copy constructor
uxdevice::painter_brush_t::paint_definition_base_t::paint_definition_base_t(
    const paint_definition_base_t &other)
    : hash_members_t(other), description(other.description),
      matrix(other.matrix), pango_color(other.pango_color),
      is_processed(other.is_processed), is_loaded(other.is_loaded) {}

/// @brief move constructor
uxdevice::painter_brush_t::paint_definition_base_t::paint_definition_base_t(
    paint_definition_base_t &&other) noexcept
    : hash_members_t(other), description(other.description),
      matrix(other.matrix), pango_color(other.pango_color),
      is_processed(other.is_processed), is_loaded(other.is_loaded) {}

/**
 * @fn uxdevice::painter_brush_t::paint_definition_base_t operator =&(const
 * paint_definition_base_t&)
 * @brief copy assignment operator
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::paint_definition_base_t &
uxdevice::painter_brush_t::paint_definition_base_t::operator=(
    const paint_definition_base_t &other) {
  hash_members_t::operator=(other);
  description = other.description;
  matrix = other.matrix;
  pango_color = other.pango_color;
  is_processed = other.is_processed;
  is_loaded = other.is_loaded;
  return *this;
}

/**
 * @fn uxdevice::painter_brush_t::paint_definition_base_t operator
 * =&(paint_definition_base_t&&)
 * @brief move assignment operator
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::paint_definition_base_t &
uxdevice::painter_brush_t::paint_definition_base_t::operator=(
    paint_definition_base_t &&other) noexcept {
  hash_members_t::operator=(other);
  description = std::move(other.description);
  matrix = std::move(other.matrix);
  pango_color = std::move(other.pango_color);
  is_processed = std::move(other.is_processed);
  is_loaded = std::move(other.is_loaded);
  return *this;
}

/**
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::painter_brush_t::paint_definition_base_t::hash_code(
    void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(painter_brush_t)), description,
               pango_color.red, pango_color.green, pango_color.blue, is_loaded);

  return __value;
}
