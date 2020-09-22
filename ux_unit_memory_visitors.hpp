/*
 * This file is part of the PLATFORM_OBJ distribution
 * {https://github.com/amatarazzo777/platform_obj). Copyright (c) 2020 Anthony
 * Matarazzo.
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
\author Anthony Matarazzo
\file uxunitmemoryvisitors.hpp
\date 9/7/20
\version 1.0
\brief interface for the platform.

*/

namespace uxdevice {




class pango_visitor_t : public emit_pango_abstract_t {
public:
  pango_visitor_t() = delete;
  pango_visitor_t(PangoLayout *_layout) : layout(_layout) {}
  void accept(void) { emit(layout); }

  void emit(PangoLayout *_layout) {
    if (fn)
      fn(_layout);
  }
  PangoLayout *layout = {};
  std::function<void(PangoLayout *_layout)> fn;
};

/**
 * \class cairo_coordinate_visitor_t
 *
 *
 *
 */
class cairo_coordinate_visitor_t : public emit_cairo_coordinate_abstract_t {
public:
  cairo_coordinate_visitor_t() = delete;
  cairo_coordinate_visitor_t(cairo_t *_cr, coordinate_t &_coord)
      : cr(_cr), coord(_coord) {}

  void accept(void);
  void emit(cairo_t *_cr);
  void emit(cairo_t *_cr, coordinate_t &a);

  cairo_t *cr = {};
  coordinate_t coord;

  std::function<void(cairo_t *_cr)> fn_v1={};
  std::function<void(cairo_t *_cr, coordinate_t &a)> fn_v2={};

};


}
