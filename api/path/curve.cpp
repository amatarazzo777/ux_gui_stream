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
 * @file ux_drawing_unit_primitives.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/relative.h>
#include <api/path/curve.h>

// clang-format on

/**
 * @fn void emit(display_context_t*)
 * @brief
 *
 * @param context
 */
void uxdevice::curve_t::emit(display_context_t *context) {
  if (context->pipeline_memory_access<relative_coordinate_t>())
    emit_relative(context->window_manager->cr);
  else
    emit_absolute(context->window_manager->cr);
}

/**
 * @internal
 * @fn curve_t::emit_absolute
 * @param cairo_t *cr
 * @brief
 * @details
 */
void uxdevice::curve_t::emit_absolute(cairo_t *cr) {
  cairo_curve_to(cr, x1, y1, x2, y2, x3, y3);
}

/**
 * @internal
 * @fn curve_t::emit_relative
 * @param cairo_t *cr
 * @brief
 * @details
 */
void uxdevice::curve_t::emit_relative(cairo_t *cr) {
  cairo_rel_curve_to(cr, x1, y1, x2, y3, x3, y3);
}
