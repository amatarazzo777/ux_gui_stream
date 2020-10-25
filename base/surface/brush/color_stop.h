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
 * @author Anthony Matarazzo
 * @file ux_painter_brush.hpp
 * @date 9/7/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {

/**
 * @class painter_brush_t
 * @brief interface for the paint class.
 */
class color_stop_t : virtual public hash_members_t {
public:
  color_stop_t(u_int32_t _c);
  color_stop_t(double _r, double _g, double _b);
  color_stop_t(const std::string &_s);
  color_stop_t(const std::string &_s, double _a);

  color_stop_t(double _o, u_int32_t _c);
  color_stop_t(double _o, double _r, double _g, double _b);
  color_stop_t(double _o, double _r, double _g, double _b, double _a);
  color_stop_t(double _o, const std::string &_s);
  color_stop_t(double _o, const std::string &_s, double _a);
  void parse_color(const std::string &_s);

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(color_stop_t)), bAutoOffset,
                 bRGBA, offset, r, g, b, a);

    return __value;
  }

  bool bAutoOffset = false;
  bool bRGBA = false;
  double offset = 0;
  double r = 0;
  double g = 0;
  double b = 0;
  double a = 1;
};

/**
 * @internal
 * @typedef color_stops_t
 * @brief
 */
typedef std::vector<color_stop_t> color_stops_t;

/**
 * @internal
 * @typedef color_stops_iterator_t
 * @brief
 */
typedef std::vector<color_stop_t>::iterator color_stops_iterator_t;
} // namespace uxdevice
