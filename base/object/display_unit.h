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
#pragma once

/**
@author Anthony Matarazzo
@file ux_display_unit.h
@date 5/12/20
@version 1.0
@details

*/

namespace uxdevice {

/**
 * @brief forward class declarations
 *
 */

/**
@internal
@class display_unit_t
@brief

@brief base class for all display units.

*/
class display_unit_t : virtual public hash_members_t,
                       virtual public system_error_t {
public:
  display_unit_t() {}

  /// @brief copy assignment operator
  display_unit_t &operator=(const display_unit_t &other);
  /// @brief move assignment
  display_unit_t &operator=(display_unit_t &&other) noexcept;

  /// @brief move constructor
  display_unit_t(display_unit_t &&other) noexcept;

  /// @brief copy constructor
  display_unit_t(const display_unit_t &other);

  virtual ~display_unit_t() {}

  void changed(void);
  bool has_changed(void);

  std::size_t hash_code(void) const noexcept;

  bool is_processed = false;
  bool bchanged = false;
};

} // namespace uxdevice
