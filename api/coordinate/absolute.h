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
@file absolute.h
@date 10/23/20
@version 1.0
@brief
*/

#pragma once

namespace uxdevice {
class hash_members_t;
class display_context_t;

/**
 * @class absolute_coordinate_t
 * @brief A marker emitter that is used to switch the context from coordinate
 * positioning. This changes the interpretation to absolute coordinates.
 *
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
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::absolute_coordinate_t)
