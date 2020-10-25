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
 @file ellipsize.h
 @date 9/7/20
 @version 1.0
 @brief
 */



namespace uxdevice {

/**
 * @class text_ellipsize_t
 * @brief
 */
class text_ellipsize_t
    : public storage_emitter_t<
          text_ellipsize_t, text_ellipsize_options_t,
          accepted_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_ellipsize_t)
