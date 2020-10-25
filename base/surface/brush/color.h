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
 * @file color.h
 * @date 9/7/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {
/**
 * @internal
 * @class
 * @brief
 */
class painter_brush_t::color_definition_t
    : public paint_definition_base_t,
      public accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
      public visitor_targets_t<all_rendering_bits> {
public:
  color_definition_t() {}

  color_definition_t(const std::string &_description, double _r, double _g,
                     double _b, double _a);

  color_definition_t(const std::string &_description);

  color_definition_t(const std::string &_description, double _a);

  color_definition_t(const color_definition_t &other);

  color_definition_t(color_definition_t &&other) noexcept;
  bool is_color_description(void);
  color_definition_t &operator=(const color_definition_t &other);
  color_definition_t &operator=(color_definition_t &&other) noexcept;

  virtual ~color_definition_t() {}

  virtual void emit(cairo_t *cr);
  std::size_t hash_code(void) const noexcept;

  double r = {};
  double g = {};
  double b = {};
  double a = 1.0;
};

} // namespace uxdevice
