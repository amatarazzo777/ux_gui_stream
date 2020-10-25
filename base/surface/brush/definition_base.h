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
 * @file painter_brush.hpp
 * @date 9/7/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {
/**
 * @internal
 * @class paint_definition_base_t
 * @brief base class for all paint brush types to inherit
 */
class painter_brush_t::paint_definition_base_t : virtual public hash_members_t {
public:
  /**
   * @fn  paint_definition_base_t()
   * @brief
   *
   */
  paint_definition_base_t() {}
  /**
   * @fn  paint_definition_base_t(const std::string&)
   * @brief
   *
   * @param _description
   */
  paint_definition_base_t(const std::string &_description)
      : description(_description) {}
  /**
   * @fn  ~paint_definition_base_t()
   * @brief
   *
   */
  virtual ~paint_definition_base_t() {}

  /// @brief copy constructor
  paint_definition_base_t(const paint_definition_base_t &other);

  /// @brief move constructor
  paint_definition_base_t(paint_definition_base_t &&other) noexcept;

  /// @brief copy assignment operator
  paint_definition_base_t &operator=(const paint_definition_base_t &other);

  /// @brief move assignment operator
  paint_definition_base_t &operator=(paint_definition_base_t &&other) noexcept;

  std::size_t hash_code(void) const noexcept;

  std::string description = {};
  matrix_t matrix = {};
  PangoColor pango_color = {0, 0, 0};
  bool is_processed = false;
  bool is_loaded = false;
};
} // namespace uxdevice
