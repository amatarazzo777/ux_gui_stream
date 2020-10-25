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
 * @file descriptive.h
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
class painter_brush_t::descriptive_definition_t
    : public paint_definition_base_t {
public:
  /// @brief default constructor
  descriptive_definition_t();

  /// @brief string
  descriptive_definition_t(const std::string &_description);

  /// @brief copy constructor
  descriptive_definition_t(const descriptive_definition_t &other);

  /// @brief move  constructor
  descriptive_definition_t(descriptive_definition_t &&other) noexcept;

  /// @brief de-constructor
  virtual ~descriptive_definition_t();
  /// @brief copy assignment
  descriptive_definition_t &operator=(const descriptive_definition_t &other);

  /// @brief move assignment
  descriptive_definition_t &
  operator=(descriptive_definition_t &&other) noexcept;

  std::size_t hash_code(void) const noexcept;
};
} // namespace uxdevice
