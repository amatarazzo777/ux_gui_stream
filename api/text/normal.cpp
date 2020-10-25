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
 * @file normal.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
// clang-format off

#include <api/text/normal.h>
#include <base/unit_object.h>

// clang-format on

/**
 * @internal
 * @fn text_render_normal_t::emit
 * @param display_context_t &context
 * @brief uses cached glyph bitmap rendering. fastest method of display using
 * bitmap font cache data. The emit function removes the text_render_path_t from
 * the display memory as they are exclusive
 * @details
 */
void uxdevice::text_render_normal_t::emit(display_context_t *context) {
  context->pipeline_memory_reset<text_render_path_t>();
}

/**
 * @internal
 * @fn text_render_normal_t::emit
 * @param cairo_t *cr
 * @param PangoLayout *layout
 * @brief invokes the pango api
 * @details

 */
void uxdevice::text_render_normal_t::emit(cairo_t *cr, PangoLayout *layout) {
  pango_cairo_show_layout(cr, layout);
}
