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
 * @file ux_display_visual.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief The display_visual_t object is a base class for objects that can be
 * drawn on the surface. The implementation provides a communication mechanism
 * between a display unit and the display_context_t. Display units that inherit
 * the object must also use the pipeline_memory_t type. The system depends on
 * the display unit to call the set_ink() function as the rectangle is needed to
 * determine where and how big the item is on the screen.
 */

// clang-format off

#include <api/options.h>
#include <base/std_base.h>
#include <base/object/layer/hash_interface.h>
#include <base/object/layer/system_error.h>
#include <base/object/layer/visitor_interface.h>
#include <base/object/display_visual.h>
#include <base/surface/draw_buffer.h>
#include <api/matrix.h>

// clang-format off

/// @brief default constructor
uxdevice::display_visual_t::display_visual_t() {}
uxdevice::display_visual_t::~display_visual_t() {}

/// @brief copy constructor
uxdevice::display_visual_t::display_visual_t(const display_visual_t &other)
    : hash_members_t(other), internal_buffer(other.internal_buffer),
      fn_cache_surface(other.fn_cache_surface),
      fn_base_surface(other.fn_base_surface), fn_draw(other.fn_draw),
      fn_draw_clipped(other.fn_draw_clipped),
      ink_rectangle(other.ink_rectangle),
      intersection_int(other.intersection_int),
      intersection_double(other.intersection_double) {}

/// @brief move constructor
uxdevice::display_visual_t::display_visual_t(display_visual_t &&other) noexcept
    : hash_members_t(std::move(other)),
      internal_buffer(std::move(other.internal_buffer)),
      fn_cache_surface(std::move(other.fn_cache_surface)),
      fn_base_surface(std::move(other.fn_base_surface)),
      fn_draw(std::move(other.fn_draw)),
      fn_draw_clipped(std::move(other.fn_draw_clipped)),
      ink_rectangle(std::move(other.ink_rectangle)),
      intersection_int(std::move(other.intersection_int)),
      intersection_double(std::move(other.intersection_double)) {}

/// @brief copy assignment operator
uxdevice::display_visual_t &
uxdevice::display_visual_t::operator=(const display_visual_t &other) {
  hash_members_t::operator=(other);
  internal_buffer = other.internal_buffer;
  fn_draw = other.fn_draw;
  fn_draw_clipped = other.fn_draw_clipped;
  fn_cache_surface = other.fn_cache_surface;
  fn_base_surface = other.fn_base_surface;
  ink_rectangle = other.ink_rectangle;
  intersection_int = other.intersection_int;
  intersection_double = other.intersection_double;
  return *this;
}

/// @brief move assignment
uxdevice::display_visual_t &
uxdevice::display_visual_t::operator=(display_visual_t &&other) noexcept {
  hash_members_t::operator=(other);
  internal_buffer = std::move(other.internal_buffer);
  fn_draw = std::move(other.fn_draw);
  fn_draw_clipped = std::move(other.fn_draw_clipped);
  fn_cache_surface = std::move(other.fn_cache_surface);
  fn_base_surface = std::move(other.fn_base_surface);
  ink_rectangle = std::move(other.ink_rectangle);
  intersection_int = std::move(other.intersection_int);
  intersection_double = std::move(other.intersection_double);
  return *this;
}

/**
 * @internal
 * @fn bool is_output(void)
 * @brief
 *
 * @return
 */
bool uxdevice::display_visual_t::is_output(void) { return true; }

/**
 * @fn bool is_valid(void)
 * @brief
 *
 * @return
 */
bool uxdevice::display_visual_t::is_valid(void) {
  return has_ink_extents && internal_buffer.is_valid() && fn_cache_surface &&
         fn_base_surface && fn_draw && fn_draw_clipped;
}

/**
 * @internal
 * @fn void set_ink(double, double, double, double)
 * @brief
 *
 * @param x
 * @param y
 * @param w
 * @param h
 */
void uxdevice::display_visual_t::set_ink(double x, double y, double w,
                                         double h) {
  ink_rectangle = {(int)x, (int)y, (int)w, (int)h};
  ink_rectangle_double = {(double)ink_rectangle.x, (double)ink_rectangle.y,
                          (double)ink_rectangle.width,
                          (double)ink_rectangle.height};
  has_ink_extents = true;
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::display_visual_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(display_visual_t)),
               has_ink_extents, overlap);
  return __value;
}

/**
 * @internal
 * @fn void changed(void)
 * @brief
 *
 */
void uxdevice::display_visual_t::changed(void) { bchanged = true; }

/**
 * @internal
 * @fn bool has_changed(void)
 * @brief
 *
 * @return
 */
bool uxdevice::display_visual_t::has_changed(void) {
  return is_different_hash();
}

/**
 * @overload
 * @nterna
 * @fn intersect
 * @param cairo_rectangle_t &r
 * @brief intersection test used by the context.
 */
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

/**
 * @internal
 * @overload
 * @fn intersect
 * @param context_cairo_region_t &rectregion
 * @brief intersection test used by the context.
 */
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

/**
 * @internal
 * @overload
 * @fn evaluate_cache
 * @param display_context_t *context
 * @brief used to determine if multiple requests are issued for the same object.
 * in which the system will develop an image of the pipeline and use the image
 * in the future rather than calling the pipeline function again. This is useful
 * for text that may move or require repaint for other purposes. However this
 * function is not only for text. All objects that implement a pipeline are
 * provided with the functionality,
 */
void uxdevice::display_visual_t::evaluate_cache(display_context_t *context) {
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

void uxdevice::display_visual_t::emit(display_context_t *context) {}
void uxdevice::display_visual_t::emit(cairo_t *cr) {}
