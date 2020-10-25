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
@file coordinate.h
@date 10/23/20
@version 1.0
@brief
*/

#pragma once

namespace uxdevice {
class hash_members_t;
class display_context_t;

/**
 * @class coordinate_t
 * @brief The api documentation for coordinate_t.
 *
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

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::coordinate_t)
