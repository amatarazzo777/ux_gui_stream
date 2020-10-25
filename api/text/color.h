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
 @file color.h
 @date 9/7/20
 @version 1.0
 @brief
 */

#pragma once

namespace uxdevice {

/**
 @class
 @brief
 */
class text_color_t
    : public painter_brush_emitter_t<
          text_color_t, visitor_targets_t<textual_render_normal_bits>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;
  void emit(cairo_t *cr) { painter_brush_emitter_t::emit(cr); }
  void emit(cairo_t *cr, coordinate_t *a) {
    painter_brush_emitter_t::emit(cr, a);
  }
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_color_t)
