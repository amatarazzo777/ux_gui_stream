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
@file line_dashes.h
@date 10/23/20
@version 1.0
@brief
*/

namespace uxdevice {

/**
 @class
 @brief
 */
class line_dashes_t
    : public class_storage_emitter_t<
          line_dashes_t, line_dash_storage_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_path_bits, image_block_bits>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
