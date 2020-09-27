/*
 * This file is part of the PLATFORM_OBJ distribution
 * {https://github.com/amatarazzo777/platform_obj). Copyright (c) 2020 Anthony
 * Matarazzo.
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
\author Anthony Matarazzo
\file ux_coordinate.hpp
\date 9/7/20
\version 1.0
\brief
*/

/**

\class coordinate_t
\brief The class is used to hold a location and clipping width, height.

\details

 */

void uxdevice::coordinate_t::emit_relative(cairo_t *cr) {
  cairo_rel_move_to(cr, x, y);
}
void uxdevice::coordinate_t::emit_absolute(cairo_t *cr) {
  cairo_move_to(cr, x, y);
}
void uxdevice::coordinate_t::emit(PangoLayout *layout) {
  if (pango_layout_get_width(layout) != w * PANGO_SCALE)
    pango_layout_set_width(layout, w * PANGO_SCALE);

  if (pango_layout_get_height(layout) != h * PANGO_SCALE)
    pango_layout_set_height(layout, h * PANGO_SCALE);
}

/**
\class relative_coordinate_t
\brief

\details

        */
void uxdevice::relative_coordinate_t::emit(display_context_t &context) {
  context.unit_memory_erase<absolute_coordinate_t>();
}

/**

\class absolute_coordinate_t
\brief

\details


 */
void uxdevice::absolute_coordinate_t::emit(display_context_t &context) {
  context.unit_memory_erase<relative_coordinate_t>();
}
