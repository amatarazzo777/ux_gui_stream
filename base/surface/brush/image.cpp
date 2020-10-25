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
 * @file image.cpp
 * @date 5/12/20
 * @version 1.0
 * @details
 */
// clang-format off

#include <base/unit_object.h>
#include "painter_brush.h"

// clang-format on

/**
 * @fn  image_block_pattern_source_definition_t()
 * @brief default constructor
 *
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
    image_block_pattern_source_definition_t() {}

/**
 * @internal
 * @fn  image_block_pattern_source_definition_t(const
 * image_block_pattern_source_definition_t&)
 * @brief copy constructor
 *
 * @param other
 */
image_block_pattern_source_definition_t(
    const image_block_pattern_source_definition_t &other)
    : paint_definition_base_t(other), image_buffer(other.image_buffer),
      pattern(other.pattern), filter(other.filter), extend(other.extend) {}

/**
 * @internal
 * @fn  image_block_pattern_source_definition_t(const
 * image_block_pattern_source_definition_t&)
 * @brief copy constructor
 *
 * @param other
 */
image_block_pattern_source_definition_t(
    const image_block_pattern_source_definition_t &other)
    : paint_definition_base_t(other), image_buffer(other.image_buffer),
      pattern(other.pattern), filter(other.filter), extend(other.extend) {}

/**
 * @internal
 * @fn
 * image_block_pattern_source_definition_t(image_block_pattern_source_definition_t&&)
 * @brief
 *
 * @param other
 */
image_block_pattern_source_definition_t(
    image_block_pattern_source_definition_t &&other) noexcept
    : paint_definition_base_t(other), image_buffer(other.image_buffer),
      pattern(other.pattern), filter(other.filter), extend(other.extend) {}

/**
 * @fn  image_block_pattern_source_definition_t(std::string&, double,
 * double, extend_options_t, filter_options_t)
 * @brief
 *
 * @param _description
 * @param _w
 * @param _h
 * @param _extend
 * @param _filter
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
    image_block_pattern_source_definition_t(std::string &_description,
                                            double _w, double _h,
                                            extend_options_t _extend,
                                            filter_options_t _filter)
    : paint_definition_base_t(_description),
      image_buffer(_description, _w, _h) {
  pattern = cairo_pattern_create_for_surface(image_buffer.rendered);
  cairo_pattern_set_extend(pattern, static_cast<cairo_extend_t>(_extend));
  cairo_pattern_set_filter(pattern, static_cast<cairo_filter_t>(_filter));
}

/**
 * @fn  image_block_pattern_source_definition_t(std::string&, double,
 * double, extend_options_t, filter_options_t)
 * @brief
 *
 * @param _description
 * @param _w
 * @param _h
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
    image_block_pattern_source_definition_t(std::string &_description,
                                            double _w, double _h)
    : paint_definition_base_t(_description),
      image_buffer(_description, _w, _h) {
  pattern = cairo_pattern_create_for_surface(image_buffer.rendered);
  cairo_pattern_set_extend(
      pattern, static_cast<cairo_extend_t>(extend_options_t::repeat));
  cairo_pattern_set_filter(pattern,
                           static_cast<cairo_filter_t>(filter_options_t::fast));
}

/**
 * @fn  image_block_pattern_source_definition_t(std::string&,
 * extend_options_t, filter_options_t)
 * @brief
 *
 * @param _description
 * @param _extend
 * @param _filter
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
    image_block_pattern_source_definition_t(std::string &_description,
                                            extend_options_t _extend,
                                            filter_options_t _filter)
    : paint_definition_base_t(_description), image_buffer(_description) {
  pattern = cairo_pattern_create_for_surface(image_buffer.rendered);
  cairo_pattern_set_extend(pattern, static_cast<cairo_extend_t>(_extend));
  cairo_pattern_set_filter(pattern, static_cast<cairo_filter_t>(_filter));
}

/**
 * @internal
 * @fn  ~image_block_pattern_source_definition_t()
 * @brief
 *
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
    ~image_block_pattern_source_definition_t() {
  if (pattern)
    cairo_pattern_destroy(pattern);
}

/**
 * @internal
 * @fn image_block_pattern_source_definition_t operator =&(const
 * image_block_pattern_source_definition_t&)
 * @brief
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t &
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::operator=(
    const image_block_pattern_source_definition_t &other) {
  image_buffer = other.image_buffer;
  pattern = other.pattern;
  filter = other.filter;
  extend = other.extend;
  return *this;
}

/**
 * @fn image_block_pattern_source_definition_t operator =&(const
 * image_block_pattern_source_definition_t&&)
 * @brief move assignment
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t &
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::operator=(
    image_block_pattern_source_definition_t &&other) noexcept {
  image_buffer = other.image_buffer;
  pattern = other.pattern;
  filter = other.filter;
  extend = other.extend;
  return *this;
}

/**
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_t::image_block_pattern_source_definition_t::emit(
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
void uxdevice::painter_brush_t::image_block_pattern_source_definition_t::emit(
    cairo_t *cr, coordinate_t *a) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::hash_code(
    void) const noexcept {
  std::size_t __value = {};

  hash_combine(__value, paint_definition_base_t::hash_code(),
               std::type_index(typeid(this)), image_buffer.hash_code(), pattern,
               filter, extend);

  return __value;
}
