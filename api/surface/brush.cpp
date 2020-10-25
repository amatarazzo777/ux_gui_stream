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
 * @file ux_surface_area_units.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
// clang-format off

#include <api/surface/brush.h>
#include <base/unit_object.h>

// clang-format on

/**
 * @internal
 * @fn surface_area_brush_t
 * @param display_context_t &context
 * @brief Sets the background brush.
 */
void uxdevice::surface_area_brush_t::emit(display_context_t *context) {
  context->window_manager->set(*this);
}
