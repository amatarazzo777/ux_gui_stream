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
 * @file radial_gradient.h
 * @date 10/23/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {
/**
 * @internal
 * @class radial_gradient_definition_t
 * @brief
 *
 */
class radial_gradient_definition_t
    : public paint_definition_base_t,
      private color_stops_provider_t,
      public accepted_interfaces_t<abstract_emit_cr_t<order_render_option>,
                                   abstract_emit_cr_a_t<order_render_option>>,
      public visitor_targets_t<all_rendering_bits> {
public:
  radial_gradient_definition_t();

  radial_gradient_definition_t(const std::string &_description, double _cx0,
                               double _cy0, double _radius0, double _cx1,
                               double _cy1, double _radius1,
                               const color_stops_t &_cs,
                               filter_options_t _filter,
                               extend_options_t _extend);

  /// @brief virtual destructor
  virtual ~radial_gradient_definition_t();

  /// @brief constructor from string
  radial_gradient_definition_t(const std::string &_description)
      : paint_definition_base_t(_description);

  /// @brief copy constructor
  radial_gradient_definition_t(const radial_gradient_definition_t &other)
      : paint_definition_base_t(other), color_stops_provider_t(other),
        cx0(other.cx0), cy0(other.cy0), radius0(other.radius0), cx1(other.cx1),
        cy1(other.cy1), radius1(other.radius1), filter(other.filter),
        extend(other.extend), pattern(other.pattern);
  /// @brief move constructor
  radial_gradient_definition_t(radial_gradient_definition_t &&other) noexcept;

  /// @brief copy assignment operator
  radial_gradient_definition_t &
  operator=(const radial_gradient_definition_t &other);

  /// @brief move assignment
  radial_gradient_definition_t &
  operator=(radial_gradient_definition_t &&other) noexcept;

  /// @brief object interface
  bool is_radial_gradient(void);
  virtual void create(void);
  virtual void emit(cairo_t *cr);
  virtual void emit(cairo_t *cr, coordinate_t *a);
  std::size_t hash_code(void) const noexcept;

  /// @brief object instance variables
  double cx0 = {};
  double cy0 = {};
  double radius0 = {};
  double cx1 = {};
  double cy1 = {};
  double radius1 = {};
  color_stops_t color_stops = {};
  filter_options_t filter = {};
  extend_options_t extend = {};
  cairo_pattern_t *pattern = {};
};
} // namespace uxdevice
