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

// clang-format off

#include <base/unit_object.h>
#include "painter_brush.h"

// clang-format on

/**
 * @fn  color_definition_t()
 * @brief default constructor
 *
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t() {}

/**
 * @fn  color_definition_t(const std::string&, double, double, double,
 * double)
 * @brief
 *
 * @param _description
 * @param _r
 * @param _g
 * @param _b
 * @param _a
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t(
    const std::string &_description, double _r, double _g, double _b, double _a)
    : paint_definition_base_t(_description), r(_r), g(_g), b(_b), a(_a) {
  is_loaded = true;
}

/**
 * @fn  color_definition_t(const std::string&)
 * @brief
 *
 * @param _description
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t(
    const std::string &_description)
    : color_definition_t(_description, 1.0) {}

/**
 * @fn  color_definition_t(const color_definition_t&)
 * @brief
 *
 * @param other
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t(
    const color_definition_t &other)
    : paint_definition_base_t(other), r(other.r), g(other.g), b(other.b),
      a(other.a) {}

/**
 * @fn  color_definition_t(color_definition_t&&)
 * @brief
 *
 * @param other
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t(
    color_definition_t &&other) noexcept
    : paint_definition_base_t(other), r(std::move(other.r)),
      g(std::move(other.g)), b(std::move(other.b)), a(std::move(other.a)) {}

uxdevice::painter_brush_t::color_definition_t::~color_definition_t() {}

/**
 * @fn  color_definition_t(const std::string&)
 * @brief
 *
 * @param _description
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t(
    const std::string &_description)
    : color_definition_t(_description, 1.0) {}

/**
 * @internal
 * @fn  color_definition_t(const std::string&, double)
 * @brief
 *
 * @param _description
 * @param _a
 *
 *
 *
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t(
    const std::string &_description, double _a)
    : paint_definition_base_t(_description) {
  if (pango_color_parse(&pango_color, _description.data())) {
    r = pango_color.red / 65535.0;
    g = pango_color.green / 65535.0;
    b = pango_color.blue / 65535.0;
    a = _a;
    is_loaded = true;
  }
}

/**
 * @fn  color_definition_t(const std::string&, double, double, double,
 * double)
 * @brief
 *
 * @param _description
 * @param _r
 * @param _g
 * @param _b
 * @param _a
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t(
    const std::string &_description, double _r, double _g, double _b, double _a)
    : paint_definition_base_t(_description), r(_r), g(_g), b(_b), a(_a) {
  is_loaded = true;
}

/**
 * @internal
 * @fn color_definition_t operator =&(const color_definition_t&)
 * @brief
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::color_definition_t &
uxdevice::painter_brush_t::color_definition_t::operator=(
    const color_definition_t &other) {
  r = other.r;
  g = other.g;
  b = other.b;
  a = other.a;
  return *this;
}

/**
 * @fn uxdevice::painter_brush_t::color_definition_t operator
 * =&(color_definition_t&&)
 * @brief
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::color_definition_t &
uxdevice::painter_brush_t::color_definition_t::operator=(
    color_definition_t &&other) noexcept {
  r = other.r;
  g = other.g;
  b = other.b;
  a = other.a;
  return *this;
}
/**
 * @internal
 * @fn bool is_color_description(void)
 * @brief
 *
 * @return
 */
bool uxdevice::painter_brush_t::color_definition_t::is_color_description(void) {
  bool bret = false;
  if (pango_color_parse(&pango_color, description.data())) {
    bret = true;
  }
  return bret;
}

/**
 * @internal
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_t::color_definition_t::emit(cairo_t *cr) {
  if (a == 1.0)
    cairo_set_source_rgb(cr, r, g, b);
  else
    cairo_set_source_rgba(cr, r, g, b, a);
}

/**
 * @fn std::size_t hash_code(void) const
 * @brief
 *
 * @return
 */
std::size_t
uxdevice::painter_brush_t::color_definition_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, paint_definition_base_t::hash_code(),
               std::type_index(typeid(this)).hash_code(), r, g, b, a);

  return __value;
}
