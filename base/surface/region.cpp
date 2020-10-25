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
 * @file region.cpp
 * @date 10/21/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include "cairo.h"
#include "region.h"

// clang-format on

uxdevice::context_cairo_region_t::context_cairo_region_t(){};

uxdevice::context_cairo_region_t::context_cairo_region_t(bool bOS, int x, int y,
                                                         int w, int h) {
  rect = {x, y, w, h};
  _rect = {(double)x, (double)y, (double)w, (double)h};
  _ptr = cairo_region_create_rectangle(&rect);
  bOSsurface = bOS;
}

uxdevice::context_cairo_region_t::context_cairo_region_t(std::size_t _obj,
                                                         int x, int y, int w,
                                                         int h)
    : obj(_obj) {
  rect = {x, y, w, h};
  _rect = {(double)x, (double)y, (double)w, (double)h};
  _ptr = cairo_region_create_rectangle(&rect);
  bOSsurface = false;
}

uxdevice::context_cairo_region_t::context_cairo_region_t(
    const context_cairo_region_t &other) {
  *this = other;
}

uxdevice::context_cairo_region_t::context_cairo_region_t &
operator=(const context_cairo_region_t &other) {
  _ptr = cairo_region_reference(other._ptr);
  rect = other.rect;
  _rect = other._rect;
  obj = other.obj;
  bOSsurface = other.bOSsurface;
  return *this;
}

uxdevice::context_cairo_region_t::~context_cairo_region_t() {
  if (_ptr)
    cairo_region_destroy(_ptr);
}
