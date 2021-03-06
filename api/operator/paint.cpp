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
 * @file paint.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 */

// clang-format off

#include <api/operator/paint.h>
#include <base/unit_object.h>


// clang-format on

/**
 * @internal
 * @fn paint_t
 * @param cairo_t *cr
 * @brief
 * @details
 */
void uxdevice::paint_t::emit(cairo_t *cr) {
  if (value == 1.0) {
    cairo_paint(cr);
  } else {
    cairo_paint_with_alpha(cr, value);
  }
}
