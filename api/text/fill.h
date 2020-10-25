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
 @file fill.h
 @date 9/7/20
 @version 1.0
 @brief
 */

namespace uxdevice {

/**
 * @class text_fill_t
 * @brief
 */
class text_fill_t
    : public painter_brush_emitter_t<
          text_fill_t,
          accepted_interfaces_t<abstract_emit_cr_a_t<order_render>>,
          visitor_targets_t<textual_render_path_bits>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;

  void emit(cairo_t *cr);
  void emit(cairo_t *cr, coordinate_t *a);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_fill_t)
