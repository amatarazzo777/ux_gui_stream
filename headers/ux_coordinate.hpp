/*
 * This file is part of the PLATFORM_OBJ distribution
 * {https://github.com/amatarazzo777/platform_obj). Copyright (c) 2020 Anthony
 * Matarazzo.
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
#pragma once

/**
\author Anthony Matarazzo
\file ux_coordinate.hpp
\date 9/24/20
\version 1.0
\brief
*/

/**

 \class coordinate_storage_t
 \brief storage class used by
 the coordinate_t object.

 \details The constructor interface is inherited by objects that are display
 units.


 */
namespace uxdevice {
class coordinate_storage_t : virtual public hash_members_t {
public:
  coordinate_storage_t() {}
  coordinate_storage_t(double _x, double _y, double _w, double _h)
      : x(_x), y(_y), w(_w), h(_h) {}
  coordinate_storage_t(double _x, double _y) : x(_x), y(_y) {}
  virtual ~coordinate_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(coordinate_storage_t)), x, y,
                 w, h);
    return __value;
  }

  double x = {};
  double y = {};
  double w = {};
  double h = {};
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::coordinate_storage_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using coordinate_t = class coordinate_t
    : public class_storage_emitter_t<
          coordinate_t, coordinate_storage_t, emit_cairo_abstract_t,
          emit_cairo_relative_coordinate_abstract_t, emit_pango_abstract_t,
          visitor_unit_memory_display_context_t,
          visitor_unit_memory_textual_render_t,
          visitor_unit_memory_image_block_t,
          pipeline_sort_order_t<order_render_option>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;
  void emit(cairo_t *cr) { emit_absolute(cr); }
  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
  void emit(PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::coordinate_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using relative_coordinate_t = class relative_coordinate_t
    : public marker_emitter_t<relative_coordinate_t,
                              emit_display_context_abstract_t,
                              visitor_unit_memory_display_context_t,
                              visitor_unit_memory_image_block_t,
                              visitor_unit_memory_textual_render_t,
                              pipeline_sort_order_t<order_render_option>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::relative_coordinate_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using absolute_coordinate_t = class absolute_coordinate_t
    : public marker_emitter_t<absolute_coordinate_t,
                              emit_display_context_abstract_t,
                              visitor_unit_memory_display_context_t,
                              visitor_unit_memory_image_block_t,
                              visitor_unit_memory_textual_render_t,
                              pipeline_sort_order_t<order_render_option>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::absolute_coordinate_t);
