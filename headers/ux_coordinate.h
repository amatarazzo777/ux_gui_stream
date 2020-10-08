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
#pragma once

/**
@author Anthony Matarazzo
@file ux_coordinate.hpp
@date 9/24/20
@version 1.0
@brief
*/

namespace uxdevice {
/**

 @class coordinate_storage_t
 @brief storage class used by
 the coordinate_t object.

 @details The constructor interface is inherited by objects that are display
 units.


 */

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

/**
 @class
 @brief
 */
class coordinate_t
    : public class_storage_emitter_t<
          coordinate_t, coordinate_storage_t,
          accepted_interfaces_t<
              abstract_emit_cr_t<order_render_option>,
              abstract_emit_cr_relative_t<order_render_option>,
              abstract_emit_layout_t<order_render_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits, image_block_bits>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;
  void emit(cairo_t *cr) { emit_absolute(cr); }
  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
  void emit(PangoLayout *layout);
};

/**
 @class
 @brief
 */
class relative_coordinate_t
    : public marker_emitter_t<
          relative_coordinate_t,
          accepted_interfaces_t<abstract_emit_context_t<order_init>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits, image_block_bits>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
};

/**
 @class
 @brief
 */
class absolute_coordinate_t
    : public marker_emitter_t<
          absolute_coordinate_t,
          accepted_interfaces_t<abstract_emit_context_t<order_init>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits, image_block_bits>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::coordinate_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::coordinate_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::relative_coordinate_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::absolute_coordinate_t);
