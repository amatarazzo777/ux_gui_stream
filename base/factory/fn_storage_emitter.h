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
@file fn_storage_emitter.h
@date 9/23/20
@version 1.0
@brief
*/

namespace uxdevice {
class hash_members_t;
class display_context_t;
class painter_brush_t;

/**
 @class
 @brief
 */
class function_object_t
    : public storage_emitter_t<
          function_object_t, cairo_function_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr) { value(cr); }
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::function_object_t)
