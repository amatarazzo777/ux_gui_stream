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
 * @file stroke_fill.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 */

// clang-format off

#include <api/operator/stroke_fill.h>
#include <api/operator/stroke_fill_storage.h>
#include <base/unit_object.h>

// clang-format on

/**
 * @internal
 * @fn stroke_fill_path_t
 * @param cairo_t *cr
 * @brief
 * @details
 */
void uxdevice::stroke_fill_path_t::emit(cairo_t *cr) {
  stroke_brush.emit(cr);
  cairo_stroke_preserve(cr);
  fill_brush.emit(cr);
  cairo_fill(cr);
}
