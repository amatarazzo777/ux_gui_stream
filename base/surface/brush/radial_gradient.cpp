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
 * @file radial_gradient.cpp
 * @date 5/12/20
 * @version 1.0
 * @details
 */
// clang-format off

#include <base/unit_object.h>
#include "painter_brush.h"

// clang-format on

/**
 * @fn  radial_gradient_definition_t()
 * @brief default constructor
 *
 */
uxdevice::painter_brush_t::radial_gradient_definition_t::
    radial_gradient_definition_t() {}

/**
 * @fn  radial_gradient_definition_t(const std::string&, double, double,
 * double, double, double, double, const color_stops_t&, filter_options_t,
 * extend_options_t)
 * @brief construction using formal binary parameters in entirety
 *
 * @param _description
 * @param _cx0
 * @param _cy0
 * @param _radius0
 * @param _cx1
 * @param _cy1
 * @param _radius1
 * @param _cs
 * @param _filter
 * @param _extend
 */
uxdevice::painter_brush_t::radial_gradient_definition_t::
    radial_gradient_definition_t(const std::string &_description, double _cx0,
                                 double _cy0, double _radius0, double _cx1,
                                 double _cy1, double _radius1,
                                 const color_stops_t &_cs,
                                 filter_options_t _filter,
                                 extend_options_t _extend)
    : paint_definition_base_t(_description), cx0(_cx0), cy0(_cy0),
      radius0(_radius0), cx1(_cx1), cy1(_cy1), radius1(_radius1),
      color_stops(_cs), filter(_filter), extend(_extend) {}

/**
 * @fn ~radial_gradient_definition_t
 * @brief destructor
 */
uxdevice::painter_brush_t::radial_gradient_definition_t::
    ~radial_gradient_definition_t() {
  if (pattern)
    cairo_pattern_destroy(pattern);
}

/// @brief constructor from string
uxdevice::painter_brush_t::radial_gradient_definition_t::
    radial_gradient_definition_t(const std::string &_description)
    : paint_definition_base_t(_description) {}

/// @brief copy constructor
uxdevice::painter_brush_t::radial_gradient_definition_t::
    radial_gradient_definition_t(const radial_gradient_definition_t &other)
    : paint_definition_base_t(other), color_stops_provider_t(other),
      cx0(other.cx0), cy0(other.cy0), radius0(other.radius0), cx1(other.cx1),
      cy1(other.cy1), radius1(other.radius1), filter(other.filter),
      extend(other.extend), pattern(other.pattern) {}

/// @brief move constructor
uxdevice::painter_brush_t::radial_gradient_definition_t::
    radial_gradient_definition_t(radial_gradient_definition_t &&other) noexcept
    : paint_definition_base_t(other), color_stops_provider_t(other),
      cx0(other.cx0), cy0(other.cy0), radius0(other.radius0), cx1(other.cx1),
      cy1(other.cy1), radius1(other.radius1), filter(other.filter),
      extend(other.extend), pattern(other.pattern) {}

/**
 * @fn operator=
 * @brief copy assignment
 * @param const radial_gradient_definition_t &other
 */
uxdevice::painter_brush_t::radial_gradient_definition_t &
uxdevice::painter_brush_t::radial_gradient_definition_t::operator=(
    const radial_gradient_definition_t &other) {
  color_stops_provider_t::operator=(other);
  cx0 = other.cx0;
  cy0 = other.cy0;
  radius0 = other.radius0;
  cx1 = other.cx1;
  cy1 = other.cy1;
  radius1 = other.radius1;
  color_stops = other.color_stops;
  filter = other.filter;
  extend = other.extend;
  pattern = other.pattern;
  return *this;
}

/**
 * @fn operator=
 * @brief copy assignment
 * @param const radial_gradient_definition_t &other
 */
uxdevice::painter_brush_t::radial_gradient_definition_t &
uxdevice::painter_brush_t::radial_gradient_definition_t::operator=(
    radial_gradient_definition_t &&other) noexcept {
  color_stops_provider_t::operator=(other);
  cx0 = other.cx0;
  cy0 = other.cy0;
  radius0 = other.radius0;
  cx1 = other.cx1;
  cy1 = other.cy1;
  radius1 = other.radius1;
  color_stops = other.color_stops;
  filter = other.filter;
  extend = other.extend;
  pattern = other.pattern;
  return *this;
}

/**
 * @fn bool is_radial_gradient(void)
 * @brief
 *
 * @return
 */
bool uxdevice::painter_brush_t::radial_gradient_definition_t::
    is_radial_gradient(void) {
  const std::string_view sRadialPattern = "radial-gradient";
  if (description.compare(0, sRadialPattern.size(), sRadialPattern) != 0)
    return false;

  return true;
}

/**
 * @fn void create(void)
 * @brief
 *
 */
void uxdevice::painter_brush_t::radial_gradient_definition_t::create(void) {
  pattern = cairo_pattern_create_radial(cx0, cy0, radius0, cx1, cy1, radius1);
  resolve_color_stops(pattern);
  cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
  is_processed = true;
  is_loaded = true;
}

/**
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_t::radial_gradient_definition_t::emit(
    cairo_t *cr) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @fn void emit(cairo_t*, coordinate_t*)
 * @brief
 *
 * @param cr
 * @param a
 */
void uxdevice::painter_brush_t::radial_gradient_definition_t::emit(
    cairo_t *cr, coordinate_t *a) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * radial_gradient.@fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::painter_brush_t::radial_gradient_definition_t::hash_code(
    void) const noexcept {
  std::size_t __value = {};

  hash_combine(__value, color_stops_provider_t::hash_code(),
               paint_definition_base_t::hash_code(),
               std::type_index(typeid(this)).hash_code(), cx0, cy0, radius0,
               cx1, cy1, radius1, filter, extend, pattern);

  return __value;
}
