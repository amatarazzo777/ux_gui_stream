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
\file ux_display_visual.cpp
\date 9/7/20
\version 1.0
\brief
*/
#include "ux_device.hpp"

void uxdevice::display_visual_t::intersect(cairo_rectangle_t &r) {
  if (!has_ink_extents)
    return;
  cairo_rectangle_int_t rInt = {(int)r.x, (int)r.y, (int)r.width,
                                (int)r.height};
  cairo_region_t *rectregion = cairo_region_create_rectangle(&rInt);
  cairo_rectangle_int_t objrect = {(int)ink_rectangle.x, (int)ink_rectangle.y,
                                   (int)ink_rectangle.width,
                                   (int)ink_rectangle.height};

  overlap = cairo_region_contains_rectangle(rectregion, &objrect);
  if (overlap == CAIRO_REGION_OVERLAP_PART) {
    cairo_region_t *dst = cairo_region_create_rectangle(&objrect);
    cairo_region_intersect(dst, rectregion);
    cairo_region_get_extents(dst, &intersection_int);
    intersection_double = {
        (double)intersection_int.x, (double)intersection_int.y,
        (double)intersection_int.width, (double)intersection_int.height};
    cairo_region_destroy(dst);
  }

  cairo_region_destroy(rectregion);
}

void uxdevice::display_visual_t::intersect(context_cairo_region_t &rectregion) {
  if (!has_ink_extents)
    return;

  cairo_region_t *dst = cairo_region_create_rectangle(&ink_rectangle);
  cairo_region_intersect(dst, rectregion._ptr);
  cairo_region_get_extents(dst, &intersection_int);
  intersection_double = {(double)intersection_int.x, (double)intersection_int.y,
                         (double)intersection_int.width,
                         (double)intersection_int.height};
  cairo_region_destroy(dst);
}

void uxdevice::display_visual_t::evaluate_cache(display_context_t &context) {
  return;
  if (bRenderBufferCached) {
    last_render_time = std::chrono::high_resolution_clock::now();
    return;
  }

  // evaluate Rendering from cache
  if (first_time_rendered) {
    first_time_rendered = false;
  }
  last_render_time = std::chrono::high_resolution_clock::now();
}
