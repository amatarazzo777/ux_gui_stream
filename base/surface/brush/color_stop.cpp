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
 * @file color_stop.cpp
 * @date 5/12/20
 * @version 1.0
 * @details
 */

// clang-format off

#include <base/std_base.h>
#include "color_stop.h"

// clang-format on

/**
 * @internal
 * @fn color_stop_t
 * @param u_int32_t _c
 * @brief color expressed as the uint32 type. Common for expressing colors in
 * hexidecimal for such as 0xFFFFFF for white or 0XFF0000 for red. RGB.
 */
uxdevice::color_stop_t::color_stop_t(u_int32_t _c) : color_stop_t(0, _c) {
  bAutoOffset = true;
}

/**
 * @internal
 * @fn color_stop_t
 * @param double _o
 * @param u_int32_t _c
 * @brief an offset specified with the color given as a uint 32 type.
 */
uxdevice::color_stop_t::color_stop_t(double _o, u_int32_t _c) {
  bRGBA = false;
  bAutoOffset = false;
  offset = _o;
  r = static_cast<u_int8_t>(_c >> 16) / 255.0;
  g = static_cast<u_int8_t>(_c >> 8) / 255.0;
  b = static_cast<u_int8_t>(_c) / 255.0;
  a = 1.0;
}

/**
 * @internal
 * @fn color_stop_t
 * @param double _r
 * @param double _g
 * @param double _b
 * @brief color given as double values 0-1.0.
 */
uxdevice::color_stop_t::color_stop_t(double _r, double _g, double _b)
    : bAutoOffset(true), bRGBA(false), offset(-1), r(_r), g(_g), b(_b), a(1) {}

/**
 * @internal
 * @fn color_stop_t
 * @param double _offset
 * @param double _r
 * @param double _g
 * @param double _b
 * @brief color specified with offset(0-1.0) , r,g,b values from 0-1.0.
 */
uxdevice::color_stop_t::color_stop_t(double _offset, double _r, double _g,
                                     double _b)
    : bAutoOffset(false), bRGBA(false), offset(_offset), r(_r), g(_g), b(_b),
      a(1) {}

/**
 * @internal
 * @fn color_stop_t
 * @param double _offset
 * @param double _r
 * @param double _g
 * @param double _b
 * @param double _a
 * @brief color offset, r,g,b,a (0-1.0)
 */
uxdevice::color_stop_t::color_stop_t(double _offset, double _r, double _g,
                                     double _b, double _a)
    : bAutoOffset(false), bRGBA(true), offset(_offset), r(_r), g(_g), b(_b),
      a(_a) {}

/**
 * @internal
 * @fn color_stop_t
 * @param const std::string &_s
 * @brief colro as a string.
 */
uxdevice::color_stop_t::color_stop_t(const std::string &_s)
    : color_stop_t(-1, _s) {
  bAutoOffset = true;
}

/**
 * @internal
 * @fn color_stop_t
 * @param const std::string &_s
 * @param double _a
 * @brief color as a string with an alpha component described in 0-1.0
 *
 */
uxdevice::color_stop_t::color_stop_t(const std::string &_s, double _a)
    : color_stop_t(-1, _s, _a) {
  bAutoOffset = true;
}

/**
 * @internal
 * @fn color_stop_t
 * @param double _o
 * @param const std::string &_s
 * @brief offset and text string color.
 */
uxdevice::color_stop_t::color_stop_t(double _o, const std::string &_s) {
  bAutoOffset = false;
  bRGBA = false;
  offset = _o;
  parse_color(_s);
}

/**
 * @internal
 * @fn color_stop_t
 * @param double _o
 * @param const std::string &_s
 * double _a
 * @brief offset, color descriptive string, with alpha
 */
uxdevice::color_stop_t::color_stop_t(double _o, const std::string &_s,
                                     double _a) {
  bAutoOffset = false;
  bRGBA = true;
  offset = _o;
  a = _a;
  parse_color(_s);
}

/**
 * @internal
 * @fn color_stop_t
 * @param const std::string &_s
 * double _a
 * @brief color parser.
 */
void uxdevice::color_stop_t::parse_color(const std::string &_s) {
  PangoColor pango_color;
  if (pango_color_parse(&pango_color, _s.data())) {
    r = pango_color.red / 65535.0;
    g = pango_color.green / 65535.0;
    b = pango_color.blue / 65535.0;
  }
}
