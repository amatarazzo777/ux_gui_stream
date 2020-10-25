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
 @file shadow.h
 @date 9/7/20
 @version 1.0
 @brief
 */

namespace uxdevice {

/**
 * @class
 * @brief
 */
class text_shadow_t
    : public painter_brush_emitter_t<
          text_shadow_t, textual_render_storage_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>,
                                abstract_emit_cr_a_t<order_render>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;

  ~text_shadow_t();

  // interface for visitors
  void emit(cairo_t *cr);
  void emit(cairo_t *cr, coordinate_t *a);

  // private functions
  void pipeline_acquire(void);
  bool pipeline_has_required_linkages(void);
};

} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_shadow_t)
