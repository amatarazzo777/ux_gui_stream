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
 * @file outline.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/coordinate.h>
#include <api/text/outline.h>

// clang-format on

/**
 * @internal
 * @fn text_outline_t::emit
 * @param cairo_t *cr
 * @brief   since class implements the painter_brush_emitter_t , specializations
 * may call the base constructor,
 *
 */
void uxdevice::text_outline_t::emit(cairo_t *cr) {
  double dwidth = cairo_get_line_width(cr);
  cairo_set_line_width(cr, _width);
  painter_brush_t::emit(cr);
  cairo_stroke(cr);
  cairo_set_line_width(cr, dwidth);
}

/**
 * @internal
 * @fn text_outline_t::emit
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief   since class implements the painter_brush_emitter_t , specializations
 * may call the base constructor,
 *
 */
void uxdevice::text_outline_t::emit(cairo_t *cr, coordinate_t *a) {
  double dwidth = cairo_get_line_width(cr);
  cairo_set_line_width(cr, _width);
  painter_brush_t::emit(cr, a);
  cairo_stroke(cr);
  cairo_set_line_width(cr, dwidth);
}
