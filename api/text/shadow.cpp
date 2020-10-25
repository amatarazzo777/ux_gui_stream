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
 * @file shadow.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/coordinate.h>
#include <api/text/shadow.h>

// clang-format on

uxdevice::text_shadow_t::~text_shadow_t() {
  if (layout)
    g_object_unref(layout);
}

/**
 * @internal
 * @fn text_shadow_t::emit
 * @param cairo_t *cr
 * @brief   s shows the text shadow
 *
 */
void uxdevice::text_shadow_t::emit(cairo_t *cr) { internal_buffer.emit(cr); }

/**
 * @internal
 * @fn text_shadow_t::emit
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief  shows the text shadow
 *
 */
void uxdevice::text_shadow_t::emit(cairo_t *cr, coordinate_t *a) {
  internal_buffer.emit(cr, a);
}

/**
 * @internal
 * @fn text_shadow_t::pipeline_acquire
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief
 *
 */
void uxdevice::text_shadow_t::pipeline_acquire() {

  pipeline_push<order_render>(fn_emit_cr_a_t{
      [&](cairo_t *cr, coordinate_t *a) {
        // draw the text in shadow color
        if (internal_buffer) {
          internal_buffer.emit(cr, a);
          return;
        }

        internal_buffer =
            draw_buffer_t(a->w + x + radius * 2, a->h + y + radius * 2);

        pipeline_disable_visit<text_shadow_t>();
        pipeline_push<order_render>(
            fn_emit_cr_a_t{[&](cairo_t *cr, coordinate_t *a) {
              internal_buffer.flush();
              internal_buffer.blur_image(radius);

              internal_buffer.emit(cr, a);
            }});
      }
      // pipeline_visit(this);();
  });
}

/**
 * @internal
 * @fn text_shadow_t::pipeline_has_required_linkages
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief determines if the textual drawing parameters for shadow are all stored
 * within the pipeline memory.
 * @return bool
 */
bool uxdevice::text_shadow_t::pipeline_has_required_linkages(void) {
  return textual_render_storage_t::pipeline_has_required_linkages();
}
