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
 * @file ux_display_visual.hpp
 * @date 9/7/20
 * @version 1.0
 * @details The display_visual_t object provides communication
 * between the display context and a display render unit. The object
 * which derives this interface should do it publicly. Another portion of the
 * construct is the rendering pipeline. Two abstract virtual functions must be
 * implemented as they are used by the display_context_t: pipeline_acquire() and
 * pipeline_has_required_linkages(). See ux_pipeline_memory.hpp.
 *
 */

#pragma once

namespace uxdevice {

/**
 * @internal
 * @typedef draw_logic_t
 * @brief
 */
typedef std::function<void(void)> draw_logic_t;

/**
 * @internal
 * @class display_visual_t
 * @brief base class for objects that produce image drawing commands.
 * The display_context_t class uses the
 * intersection, ink, and evaluate cache functions. Intersection,ink
 *  to determine if a particular derived drawing object is on
 * screen.

 */
class hash_members_t;
class display_context_t;
class draw_buffer_t;
class context_cairo_region_t;

class display_visual_t : virtual public hash_members_t {
public:
  /// @brief default constructor
  display_visual_t();
  virtual ~display_visual_t();

  /// @brief copy constructor
  display_visual_t(const display_visual_t &other);

  /// @brief move constructor
  display_visual_t(display_visual_t &&other) noexcept;

  /// @brief copy assignment operator
  display_visual_t &operator=(const display_visual_t &other);

  /// @brief move assignment
  display_visual_t &operator=(display_visual_t &&other) noexcept;

  /// @brief initialization function which typically links to pipeline memory
  void emit(display_context_t *context);

  /// @brief generalized drawing function. Within this context, the coordinates,
  /// a basis of drawing operation is within the pipeline memory for the object.
  /// Or the object has otherways of calculating the coordinates.
  void emit(cairo_t *cr);

  /// @brief a function that implements a cache to image if the object is
  /// requested to render again in less that a time period. If so, this function
  /// provides the switchover in resources. The result is a faster but more
  /// memory usage. Especially for text areas.
  void evaluate_cache(display_context_t *context);

  void intersect(cairo_rectangle_t &r);
  void intersect(context_cairo_region_t &r);

  /// @brief query function for valid state checking.
  virtual bool is_valid(void);

  /// @brief an import function is is a requirement before the item may be
  /// displayed. Within the emit initialization, the set ink function must be
  /// called.
  void set_ink(double x, double y, double w, double h);

  // implementation
  std::size_t hash_code(void) const noexcept;

  /// @brief state switch to bypass hash code if set while maintaining hash
  /// consistency of change detection.
  void changed(void);

  /// @brief an important function that is the summary of all the hash or query
  /// change events for the object. The function returns true if prior changed()
  /// was called. Otherwiae it checks the hash code if the object has that
  /// interface.
  bool has_changed(void);

  /// @brief state
  bool bchanged = false;
  bool has_ink_extents = false;
  cairo_rectangle_int_t c = {};
  cairo_region_overlap_t overlap = CAIRO_REGION_OVERLAP_OUT;

  std::atomic<bool> bRenderBufferCached = false;
  draw_buffer_t internal_buffer = {};

  /**
   * Set up place for the interface for these item to lock the functions
   * while they might be inside a call....................
   *
   */
  draw_logic_t fn_cache_surface = {};
  draw_logic_t fn_base_surface = {};
  draw_logic_t fn_draw = {};
  draw_logic_t fn_draw_clipped = {};
  matrix_t matrix = {};

  // measure processing time
  std::chrono::system_clock::time_point last_render_time = {};

  bool first_time_rendered = true;

  cairo_rectangle_int_t ink_rectangle = cairo_rectangle_int_t();
  cairo_rectangle_t ink_rectangle_double = cairo_rectangle_t();
  cairo_rectangle_int_t intersection_int = cairo_rectangle_int_t();
  cairo_rectangle_t intersection_double = cairo_rectangle_t();
};

/**
 * @internal
 * @typedef display_visual_list_t
 * @brief used to hold the list of visual. the display context uses this type.
 *
 */
typedef std::list<std::shared_ptr<display_visual_t>> display_visual_list_t;

} // namespace uxdevice
