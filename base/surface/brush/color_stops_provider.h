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
 * @file color_stops_provider.h
 * @date 9/7/20
 * @version 1.0
 * @brief
 */

/**
 * @class color_stops_provider_t
 * @brief resolves filling and auto calculated values. This reduces the
 * parameters needed to create multiple stops allowing for different types of
 * attributes to be filled in such as offset.
 *
 *
 */
class color_stops_provider_t : virtual public hash_members_t {
public:
  /// @brief default constructor
  color_stops_provider_t();
  virtual ~color_stops_provider_t();

  /// @brief constructor accepting unprocessed color_stops sent by api call.
  // color_stops_provider_t(const color_stops_t &_cs) : color_stops(_cs) {}

  /// @brief copy constructor
  color_stops_provider_t(const color_stops_provider_t &other);

  /// @brief move constructor
  color_stops_provider_t(color_stops_provider_t &&other) noexcept;

  /// @brief copy assignment
  color_stops_provider_t &operator=(const color_stops_provider_t &other);

  /// @brief move assignment
  color_stops_provider_t &operator=(color_stops_provider_t &&other) noexcept;

  /// @brief process
  void resolve_color_stops(cairo_pattern_t *pattern);

  /// @brief hash of all items in color_stops.
  std::size_t hash_code(void) const noexcept;

  color_stops_t color_stops = {};
};

/**
 parse web formats

linear-gradient(to bottom, #1e5799 0%,#2989d8 50%,#207cca 51%,#2989d8
51%,#7db9e8 100%);

linear-gradient(to right, #1e5799 0%,#2989d8 50%,#207cca
51%,#2989d8 51%,#7db9e8 100%);

linear-gradient(135deg, #1e5799 0%,#2989d8
50%,#207cca 51%,#2989d8 51%,#7db9e8 100%);

linear-gradient(45deg, #1e5799
0%,#2989d8 50%,#207cca 51%,#2989d8 51%,#7db9e8 100%);

radial-gradient(ellipsize at center, #1e5799 0%,#2989d8 50%,#207cca
51%,#2989d8 51%,#7db9e8 100%);

*/
