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
 @file normal.h
 @date 9/7/20
 @version 1.0
 @brief
 */

namespace uxdevice {

/**
 * @class text_render_normal_t
 * @brief
 */
class text_render_normal_t
    : public marker_emitter_t<
          text_render_normal_t,
          accepted_interfaces_t<abstract_emit_context_t<order_init>,
                                abstract_emit_cr_layout_t<order_render>>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
  void emit(cairo_t *cr, PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_render_normal_t)
