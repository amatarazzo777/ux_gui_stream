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
 * @file vline.cpp
 * @date 9/19/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/relative.h>
#include <api/path/vline.h>
// clang-format on

/**
 * @fn void emit(display_context_t*)
 * @brief
 *
 * @param context
 */
void uxdevice::vline_t::emit(display_context_t *context) {
  if (context->pipeline_memory_access<relative_coordinate_t>())
    emit_relative(context->window_manager->cr);
  else
    emit_absolute(context->window_manager->cr);
}

/**
 * @internal
 * @fn vline_t
 * @param cairo_t *cr
 * @brief
 * @details
 */
void uxdevice::vline_t::emit_relative(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_rel_line_to(cr, 0, value);
  }
}

/**
 * @internal
 * @fn emit_absolute::emit_absolute
 * @param cairo_t *cr
 * @brief
 * @details
 */
void uxdevice::vline_t::emit_absolute(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_line_to(cr, curx, value);
  }
}
