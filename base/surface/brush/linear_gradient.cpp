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
 * @file linear_gradient.cpp
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
 * @fn  linear_gradient_definition_t()
 * @brief default constructor
 *
 */
uxdevice::painter_brush_t::linear_gradient_definition_t::
    linear_gradient_definition_t() {}

/**
 * @fn  linear_gradient_definition_t(const std::string&)
 * @brief creation from a description
 *
 * @param _description
 */
uxdevice::painter_brush_t::linear_gradient_definition_t::
    linear_gradient_definition_t(const std::string &_description)
    : paint_definition_base_t(_description) {}

/**
 * @fn  linear_gradient_definition_t(const std::string&, double, double,
 * double, double, const color_stops_t&, filter_options_t, extend_options_t)
 * @brief formal parameters in binary entirety
 *
 * @param _description
 * @param _x0
 * @param _y0
 * @param _x1
 * @param _y1
 * @param _cs
 * @param _filter
 * @param _extend
 */
uxdevice::painter_brush_t::linear_gradient_definition_t::
    linear_gradient_definition_t(const std::string &_description, double _x0,
                                 double _y0, double _x1, double _y1,
                                 const color_stops_t &_cs,
                                 filter_options_t _filter,
                                 extend_options_t _extend)
    : paint_definition_base_t(_description), x0(_x0), y0(_y0), x1(_x1), y1(_y1),
      filter(_filter), extend(_extend) {
  create();
}

/**
 * @fn  ~linear_gradient_definition_t()
 * @brief
 *
 */
uxdevice::painter_brush_t::linear_gradient_definition_t::
    ~linear_gradient_definition_t() {
  if (pattern)
    cairo_pattern_destroy(pattern);
}

/// @brief copy constructor
uxdevice::painter_brush_t::linear_gradient_definition_t::
    linear_gradient_definition_t(const linear_gradient_definition_t &other)
    : color_stops_provider_t(other), paint_definition_base_t(other),
      x0(other.x0), y0(other.y0), x1(other.x1), y1(other.y1),
      filter(other.filter), extend(other.extend),
      pattern(cairo_pattern_reference(other.pattern)) {}

/// @brief move constructor
uxdevice::painter_brush_t::linear_gradient_definition_t::
    linear_gradient_definition_t(linear_gradient_definition_t &&other) noexcept
    : color_stops_provider_t(other), paint_definition_base_t(other),
      x0(other.x0), y0(other.y0), x1(other.x1), y1(other.y1),
      filter(other.filter), extend(other.extend), pattern(other.pattern) {}

/**
 * @brief copy assignment
 */
uxdevice::painter_brush_t::linear_gradient_definition_t &
uxdevice::painter_brush_t::linear_gradient_definition_t::operator=(
    const linear_gradient_definition_t &other) {
  color_stops_provider_t::operator=(other);
  paint_definition_base_t::operator=(other);
  x0 = other.x0;
  y0 = other.y0;
  x1 = other.x1;
  y1 = other.y1;
  color_stops = other.color_stops;
  filter = other.filter;
  extend = other.extend;
  pattern = cairo_pattern_reference(other.pattern);
  return *this;
}

/**
 * move assignment
 */
uxdevice::painter_brush_t::linear_gradient_definition_t &
uxdevice::painter_brush_t::linear_gradient_definition_t::operator=(
    linear_gradient_definition_t &&other) noexcept {
  color_stops_provider_t::operator=(other);
  paint_definition_base_t::operator=(other);
  x0 = other.x0;
  y0 = other.y0;
  x1 = other.x1;
  y1 = other.y1;
  color_stops = other.color_stops;
  filter = other.filter;
  extend = other.extend;
  pattern = other.pattern;
  return *this;
}

/**
 * @fn bool is_linear_gradient_description(void)
 * @brief
 *
 * @return
 */
bool uxdevice::painter_brush_t::linear_gradient_definition_t::
    is_linear_gradient_description(void) {
  const std::string_view sLinearPattern = "linear-gradient";
  if (description.compare(0, sLinearPattern.size(), sLinearPattern) != 0)
    return false;

  return true;
}

/**
 * @internal
 * @fn void create(void)
 * @brief
 *
 */
void uxdevice::painter_brush_t::linear_gradient_definition_t::create(void) {
  pattern = cairo_pattern_create_linear(x0, y0, x1, y1);
  resolve_color_stops(pattern);
  cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
  is_processed = true;
  is_loaded = true;
}

/**
 * @internal
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_t::linear_gradient_definition_t::emit(
    cairo_t *cr) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @internal
 * @fn void emit(cairo_t*, coordinate_t*)
 * @brief
 *
 * @param cr
 * @param a
 */
void uxdevice::painter_brush_t::linear_gradient_definition_t::emit(
    cairo_t *cr, coordinate_t *a) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief hash of unit including color stops.
 *
 * @return value std::size_t
 */
std::size_t uxdevice::painter_brush_t::linear_gradient_definition_t::hash_code(
    void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, color_stops_provider_t::hash_code(),
               paint_definition_base_t::hash_code(),
               std::type_index(typeid(this)).hash_code(), x0, y0, x1, y1,
               filter, extend, pattern);

  return __value;
}
