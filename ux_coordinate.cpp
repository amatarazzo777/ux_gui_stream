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
 * @file ux_coordinate.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief implementation of the coordinate_t visit interfaces. The class is used
 * to hold a location and clipping width, height.
 */

#include <ux_device.h>

/**
 * @internal
 * @fn emit_relative
 * @param cairo_t *cr
 * @brief performs a relative coordinate move
 */
void uxdevice::coordinate_t::emit_relative(cairo_t *cr) {
  cairo_rel_move_to(cr, x, y);
}

/**
 * @internal
 * @fn emit_absolute
 * @param cairo_t *cr
 * @brief performs an absolute coordinate location.
 */
void uxdevice::coordinate_t::emit_absolute(cairo_t *cr) {
  cairo_move_to(cr, x, y);
}

/**
 * @internal
 * @fn emit
 * @param PangoLayout *layout
 * @brief emits the coordinates to the layout. The layout is primarily concerned
 * with the width and height of the requested layout.
 */
void uxdevice::coordinate_t::emit(PangoLayout *layout) {
  if (pango_layout_get_width(layout) != w * PANGO_SCALE)
    pango_layout_set_width(layout, w * PANGO_SCALE);

  if (pango_layout_get_height(layout) != h * PANGO_SCALE)
    pango_layout_set_height(layout, h * PANGO_SCALE);
}

/**
 * @internal
 * @fn emit
 * @param display_context_t *context
 * @brief
 */
void uxdevice::relative_coordinate_t::emit(display_context_t *context) {
  context->pipeline_memory_reset<absolute_coordinate_t>();
}

/**
 * @internal
 * @fn emit
 * @param display_context_t *context
 * @brief
 */
void uxdevice::absolute_coordinate_t::emit(display_context_t *context) {
  context->pipeline_memory_reset<relative_coordinate_t>();
}
