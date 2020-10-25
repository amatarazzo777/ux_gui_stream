#pragma once

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
 * @file linear_gradient.h
 * @date 10/23/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {
/**
 * @internal
 * @class
 * @brief
 */
class painter_brush_t::linear_gradient_definition_t
    : private color_stops_provider_t,
      public paint_definition_base_t,
      public accepted_interfaces_t<abstract_emit_cr_t<order_render_option>,
                                   abstract_emit_cr_a_t<order_render_option>>,
      public visitor_targets_t<all_rendering_bits> {
public:
  /**
   * @fn  linear_gradient_definition_t()
   * @brief default constructor
   *
   */
  linear_gradient_definition_t() {}

  linear_gradient_definition_t(const std::string &_description)
      : paint_definition_base_t(_description);

  linear_gradient_definition_t(const std::string &_description, double _x0,
                               double _y0, double _x1, double _y1,
                               const color_stops_t &_cs,
                               filter_options_t _filter,
                               extend_options_t _extend);

  /// @virtual destructor
  virtual ~linear_gradient_definition_t();

  /// @brief copy constructor
  linear_gradient_definition_t(const linear_gradient_definition_t &other);

  /// @brief move constructor
  linear_gradient_definition_t(linear_gradient_definition_t &&other) noexcept;
  /// @brief copy assignment
  linear_gradient_definition_t &
  operator=(const linear_gradient_definition_t &other);

  /// @brief move assignment
  linear_gradient_definition_t &
  operator=(linear_gradient_definition_t &&other) noexcept;

  /// @brief interface routines
  bool is_linear_gradient_description(void);
  void create(void);
  virtual void emit(cairo_t *cr);
  virtual void emit(cairo_t *cr, coordinate_t *a);
  std::size_t hash_code(void) const noexcept;

  /// @brief object instance variables
  double x0 = {};
  double y0 = {};
  double x1 = {};
  double y1 = {};

  filter_options_t filter = {};
  extend_options_t extend = {};
  cairo_pattern_t *pattern = {};
};
} // namespace uxdevice
