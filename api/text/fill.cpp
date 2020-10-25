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
 * @file fill.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/coordinate.h>
#include <api/text/fill.h>

// clang-format on

void uxdevice::text_fill_t::emit(cairo_t *cr) {
  painter_brush_t::emit(cr);
  cairo_fill_preserve(cr);
}

void uxdevice::text_fill_t::emit(cairo_t *cr, coordinate_t *a) {
  painter_brush_t::emit(cr, a);
  cairo_fill_preserve(cr);
}
