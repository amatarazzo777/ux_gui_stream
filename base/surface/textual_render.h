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
 * @author Anthony Matarazzo
 * @file textual_render.h
 * @date 9/19/20
 * @version 1.0
 * @brief
 */

#pragma once

namespace uxdevice {

/**
 * @class textual_render_t
 * @brief
 */
class textual_render_t
    : public class_storage_emitter_t<
          textual_render_t, textual_render_storage_t,
          accepted_interfaces_t<abstract_emit_context_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(display_context_t *context);
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::textual_render_t)
