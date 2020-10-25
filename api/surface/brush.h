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

namespace uxdevice {

/**
 * @class surface_area_brush_t
 * @brief stores and emits the background of the surface. The class inherits
 * from painter_brush_emitter_t.
 */
class surface_area_brush_t
    : public painter_brush_emitter_t<
          surface_area_brush_t,
          accepted_interfaces_t<abstract_emit_context_t<order_init>>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;

  void emit(display_context_t *context);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::surface_area_brush_t)
