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
\file uxdisplayunits.hpp
\date 9/7/20
\version 1.0
\brief
*/
/**
\author Anthony Matarazzo
\file uxdisplaycontext.hpp
\date 5/12/20
\version 1.0
 \details CLass holds the display window context, gui drawing, cairo
 context, and provides an interface for threads running to
 invalidate part of the surface, resize the surface. The
 unit_memory_t class is within the public members and holds
 the state of the last used data parameters.

*/
#pragma once

/**
\internal
\class display_visual_t
\brief base class for objects that produce image drawing commands
The is_output is overridden to return true. As well the object uses
the intersection to determine if a particular derrived drawing object
is on screen.

\details


 */
namespace uxdevice {
class context_cairo_region_t {
public:
  context_cairo_region_t() = delete;
  context_cairo_region_t(bool bOS, int x, int y, int w, int h) {
    rect = {x, y, w, h};
    _rect = {(double)x, (double)y, (double)w, (double)h};
    _ptr = cairo_region_create_rectangle(&rect);
    bOSsurface = bOS;
  }
  context_cairo_region_t(std::size_t _obj, int x, int y, int w, int h)
      : obj(_obj) {
    rect = {x, y, w, h};
    _rect = {(double)x, (double)y, (double)w, (double)h};
    _ptr = cairo_region_create_rectangle(&rect);
    bOSsurface = false;
  }

  context_cairo_region_t(const context_cairo_region_t &other) { *this = other; }
  context_cairo_region_t &operator=(const context_cairo_region_t &other) {
    _ptr = cairo_region_reference(other._ptr);
    rect = other.rect;
    _rect = other._rect;
    obj = other.obj;
    bOSsurface = other.bOSsurface;
    return *this;
  }
  ~context_cairo_region_t() {
    if (_ptr)
      cairo_region_destroy(_ptr);
  }
  cairo_rectangle_int_t rect = cairo_rectangle_int_t();
  cairo_rectangle_t _rect = cairo_rectangle_t();
  cairo_region_t *_ptr = nullptr;
  std::size_t obj = 0;
  bool bOSsurface = false;
};

typedef std::function<void(void)> draw_logic_t;

class display_visual_t : virtual public hash_members_t {
public:
  /// @brief default constructor
  display_visual_t() {}

  /// @brief move assignment
  display_visual_t &operator=(display_visual_t &&other) noexcept {

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

  /// @brief copy assignment operator
  display_visual_t &operator=(const display_visual_t &other) {

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

  /// @brief move constructor
  display_visual_t(display_visual_t &&other) noexcept
      : internal_buffer(std::move(other.internal_buffer)),

        fn_cache_surface(std::move(other.fn_cache_surface)),
        fn_base_surface(std::move(other.fn_base_surface)),
        fn_draw(std::move(other.fn_draw)),
        fn_draw_clipped(std::move(other.fn_draw_clipped)),
        ink_rectangle(std::move(other.ink_rectangle)),
        intersection_int(std::move(other.intersection_int)),
        intersection_double(std::move(other.intersection_double)) {}

  /// @brief copy constructor
  display_visual_t(const display_visual_t &other) {
    // invoke copy operator
    *this = other;
  }

  virtual ~display_visual_t() {}
  void emit(cairo_t *cr);
  void intersect(cairo_rectangle_t &r);
  void intersect(context_cairo_region_t &r);

  bool is_output(void) { return true; }
  virtual bool is_valid(void) {
    return has_ink_extents && internal_buffer.is_valid() && fn_cache_surface &&
           fn_base_surface && fn_draw && fn_draw_clipped;
  }

  void set_ink(double x, double y, double w, double h) {
    ink_rectangle = {(int)x, (int)y, (int)w, (int)h};
    ink_rectangle_double = {(double)ink_rectangle.x, (double)ink_rectangle.y,
                            (double)ink_rectangle.width,
                            (double)ink_rectangle.height};
    has_ink_extents = true;
  }

  // These functions switch the rendering apparatus from off
  // screen threaded to on screen. all rendering is serialize to the main
  // surface
  //
  std::atomic_flag lockFunctors = ATOMIC_FLAG_INIT;
#define LOCK_FUNCTORS_SPIN                                                     \
  while (lockFunctors.test_and_set(std::memory_order_acquire))

#define LOCK_FUNCTORS_CLEAR lockFunctors.clear(std::memory_order_release)

  void functors_lock(bool b) {
    if (b)
      LOCK_FUNCTORS_SPIN;
    else
      LOCK_FUNCTORS_CLEAR;
  }
  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(display_visual_t)),
                 has_ink_extents, overlap);
    return __value;
  }

  void changed(void) { bchanged = true; }
  bool has_changed(void) { return is_different_hash(); }

  bool is_processed = false;
  bool bchanged = false;
  bool has_ink_extents = false;
  cairo_rectangle_int_t c = cairo_rectangle_int_t();
  cairo_region_overlap_t overlap = CAIRO_REGION_OVERLAP_OUT;

  std::atomic<bool> bRenderBufferCached = false;
  draw_buffer_t internal_buffer = {};

  /**
   * Set up place for the interface for these item to lock the functions
   * while they might be inside a call....................
   *
   */
  draw_logic_t fn_cache_surface = draw_logic_t();
  draw_logic_t fn_base_surface = draw_logic_t();
  draw_logic_t fn_draw = draw_logic_t();
  draw_logic_t fn_draw_clipped = draw_logic_t();

  // measure processing time
  std::chrono::system_clock::time_point last_render_time = {};
  void evaluate_cache(display_context_t *context);
  bool first_time_rendered = true;

  cairo_rectangle_int_t ink_rectangle = cairo_rectangle_int_t();
  cairo_rectangle_t ink_rectangle_double = cairo_rectangle_t();
  cairo_rectangle_int_t intersection_int = cairo_rectangle_int_t();
  cairo_rectangle_t intersection_double = cairo_rectangle_t();
};
} // namespace uxdevice
