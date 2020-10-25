#pragma once

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
 @author Anthony Matarazzo
 @file outline.h
 @date 9/7/20
 @version 1.0
 @brief
 */

namespace uxdevice {

/**
 * @class text_outline_t
 * @brief
 */
class text_outline_t
    : public painter_brush_emitter_t<
          text_outline_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>,
                                abstract_emit_cr_a_t<order_render>>,
          visitor_targets_t<textual_render_path_bits>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;

  // since class implements the painter_brush_emitter_t, specializations may
  // call the base constructor, that supply the painter_brush_emitter interface
  // that is inherited. The emitter functions should call the
  // base class members.
  void emit(cairo_t *cr);
  void emit(cairo_t *cr, coordinate_t *a);

  // local property that returns the same object. Continuation syntax for
  // in-line options.
  text_outline_t &width(double __width) {
    _width = __width;
    return *this;
  }
  double _width = 1;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_outline_t)
