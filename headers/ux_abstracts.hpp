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
\file ux_abstracts.hpp
\date 9/23/20
\version 1.0
\brief
*/

/**

\internal
\brief
  Abstract interfaces that should be defined within the class definition
template parameters of the object factories. When these are placed there, the
public members of the pure abstract class are inherited within the resulting
class generated  from the template. This allows one to implement the function
for callback or usage where appropriate. The uxdisplayunits.cpp file contains
the implementation of these objects. Objects are of a trivial storage or class
type.  These methods provide usage for the data or marking the position within
the display list.

*/
namespace uxdevice {
class display_context_t;
class coordinate_t;

class emit_display_context_abstract_t {
public:
  virtual ~emit_display_context_abstract_t(){};
  virtual void emit(display_context_t *context) = 0;
};

} // namespace uxdevice

namespace uxdevice {
class emit_cairo_abstract_t {
public:
  virtual ~emit_cairo_abstract_t(){};
  virtual void emit(cairo_t *cr) = 0;
};

} // namespace uxdevice

namespace uxdevice {
class emit_cairo_relative_coordinate_abstract_t {
public:
  virtual ~emit_cairo_relative_coordinate_abstract_t(){};
  virtual void emit_relative(cairo_t *cr) = 0;
  virtual void emit_absolute(cairo_t *cr) = 0;
};

} // namespace uxdevice

namespace uxdevice {
class emit_cairo_coordinate_abstract_t {
public:
  virtual ~emit_cairo_coordinate_abstract_t(){};
  virtual void emit(cairo_t *cr) = 0;
  virtual void emit(cairo_t *cr, coordinate_t *a) = 0;
};

} // namespace uxdevice

namespace uxdevice {
class emit_pango_abstract_t {
public:
  virtual ~emit_pango_abstract_t(){};
  virtual void emit(PangoLayout *layout) = 0;
};
} // namespace uxdevice

namespace uxdevice {
class attribute_display_context_memory_t {
public:
  virtual ~attribute_display_context_memory_t() {}
};
} // namespace uxdevice

// abstract labels class as copy and emit local unit memory
namespace uxdevice {
class visitor_unit_memory_display_context_t {
public:
  virtual ~visitor_unit_memory_display_context_t() {}
};
} // namespace uxdevice

// abstract labels class as copy and emit local unit memory
namespace uxdevice {
class visitor_unit_memory_textual_render_t {
public:
  virtual ~visitor_unit_memory_textual_render_t() {}
};
} // namespace uxdevice

namespace uxdevice {
class visitor_unit_memory_image_block_t {
public:
  virtual ~visitor_unit_memory_image_block_t() {}
};
} // namespace uxdevice
