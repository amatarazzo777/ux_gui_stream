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

/**
 * forward class declarations.
 */
namespace uxdevice {
class display_context_t;
class coordinate_t;
} // namespace uxdevice

/**
 * function that compose each of the interface class. some interfaces have two
 * functions that are overloaded. Yet the meaning and implementation of the
 * function is specific to the implementation according to use within the
 * rendering pipeline. Each of the functions are labeled as being a part of a
 * specific rendering stage within the declaration of the unit.
 */
namespace uxdevice {
using fn_emit_cr_t = std::function<void(cairo_t *)>;
using fn_emit_cr_a_t = std::function<void(cairo_t *, coordinate_t *)>;
using fn_emit_cr_a_absolute_t = std::function<void(cairo_t *, coordinate_t *)>;
using fn_emit_cr_a_relative_t = std::function<void(cairo_t *, coordinate_t *)>;
using fn_emit_context_t = std::function<void(display_context_t *context)>;
using fn_emit_layout_t = std::function<void(PangoLayout *)>;
using fn_emit_layout_a_t = std::function<void(PangoLayout *, coordinate_t *)>;
using fn_emit_cr_layout_t = std::function<void(cairo_t *cr, PangoLayout *)>;

using fn_emit_overload_t =
    std::variant<std::monostate, fn_emit_cr_t, fn_emit_cr_a_t,
                 fn_emit_cr_a_absolute_t, fn_emit_cr_a_relative_t,
                 fn_emit_context_t, fn_emit_layout_t, fn_emit_layout_a_t,
                 fn_emit_cr_layout_t>;
} // namespace uxdevice

/**
 * \class visitor_interface_t
 * \brief holds information for the visitor by type index.
 */
namespace uxdevice {
class visitor_interface_t {
public:
  std::type_index type=std::type_index(typeid(visitor_interface_t));
  std::size_t pipeline_order;
  fn_emit_overload_t fn;
};
} // namespace uxdevice

namespace uxdevice {
template <typename FN, std::size_t ORDER>
visitor_interface_t make_interface_t(FN fn) {
  return visitor_interface_t{std::type_index(typeid(FN)), ORDER, fn_emit_overload_t{fn}};
}

} // namespace uxdevice

namespace uxdevice {
template <std::size_t ORDER> class abstract_emit_context_t {
public:
  abstract_emit_context_t() {}
  virtual ~abstract_emit_context_t() {}
  virtual void emit(display_context_t *context) = 0;

  visitor_interface_t interface = make_interface_t<fn_emit_context_t, ORDER>(
      std::bind(&abstract_emit_context_t::emit, this, std::placeholders::_1));

}; // namespace uxdevice
} // namespace uxdevice

namespace uxdevice {
template <std::size_t ORDER> class abstract_emit_cr_t {
public:
  abstract_emit_cr_t() {}
  virtual ~abstract_emit_cr_t() {}
  virtual void emit(cairo_t *cr) = 0;

  visitor_interface_t fn_interface = make_interface_t<fn_emit_cr_t, ORDER>(
      std::bind(&abstract_emit_cr_t::emit, this, std::placeholders::_1));
};

} // namespace uxdevice

namespace uxdevice {
template <std::size_t ORDER> class abstract_emit_cr_absolute_t {
public:
  abstract_emit_cr_absolute_t() {}
  virtual ~abstract_emit_cr_absolute_t() {}
  virtual void emit_absolute(cairo_t *cr) = 0;

  visitor_interface_t fn_interface =
      make_interface_t<fn_emit_cr_a_absolute_t, ORDER>(
          std::bind(&abstract_emit_cr_absolute_t::emit_absolute, this,
                    std::placeholders::_1));
};
} // namespace uxdevice

namespace uxdevice {
template <std::size_t ORDER> class abstract_emit_cr_relative_t {
public:
  abstract_emit_cr_relative_t() {}
  virtual ~abstract_emit_cr_relative_t() {}
  virtual void emit_relative(cairo_t *cr) = 0;

  visitor_interface_t fn_interface = {

      make_interface_t<fn_emit_cr_a_relative_t, ORDER>(
          std::bind(&abstract_emit_cr_relative_t::emit_relative, this,
                    std::placeholders::_1))};
};
} // namespace uxdevice

namespace uxdevice {
template <std::size_t ORDER> class abstract_emit_cr_a_t {
public:
  abstract_emit_cr_a_t() {}
  virtual ~abstract_emit_cr_a_t() {}
  virtual void emit(cairo_t *cr, coordinate_t *a) = 0;

  /**
   * note: the parameters have to be explicitly named
   * to select the correct overload.
   */
  visitor_interface_t fn_interface=visitor_interface_t{};
#if 0
  visitor_interface_t fn_interface = make_interface_t<fn_emit_cr_a_t, ORDER>(
      std::bind((void (abstract_emit_cr_a_t::*)(cairo_t *cr, coordinate_t *a)) &
                    abstract_emit_cr_a_t::emit,*
                this, std::placeholders::_1, std::placeholders::_2));
#endif
};
} // namespace uxdevice

namespace uxdevice {
template <std::size_t ORDER> class abstract_emit_layout_t {
public:
  abstract_emit_layout_t() {}
  virtual ~abstract_emit_layout_t() {}
  virtual void emit(PangoLayout *layout) = 0;

  visitor_interface_t fn_interface = make_interface_t<fn_emit_layout_t, ORDER>(
      std::bind((void (abstract_emit_layout_t::*)(PangoLayout *layout)) &
                    abstract_emit_layout_t::emit,
                this, std::placeholders::_1));
};
} // namespace uxdevice

namespace uxdevice {
template <std::size_t ORDER> class abstract_emit_layout_a_t {
public:
  abstract_emit_layout_a_t() {}
  virtual ~abstract_emit_layout_a_t(){};
  virtual void emit(PangoLayout *layout, coordinate_t *a) = 0;

  visitor_interface_t fn_interface = make_interface_t<fn_emit_cr_a_t, ORDER>(
      std::bind((void (abstract_emit_layout_a_t::*)(PangoLayout *layout,
                                                    coordinate_t *a)) &
                    abstract_emit_layout_a_t::emit,
                this, std::placeholders::_1, std::placeholders::_2));
};
} // namespace uxdevice

namespace uxdevice {
template <std::size_t ORDER> class abstract_emit_cr_layout_t {
public:
  abstract_emit_cr_layout_t() {}
  virtual ~abstract_emit_cr_layout_t() {}
  virtual void emit(cairo_t *cr, PangoLayout *layout) = 0;

  visitor_interface_t fn_interface =
      make_interface_t<fn_emit_cr_layout_t, ORDER>(
          std::bind((void (abstract_emit_cr_layout_t::*)(cairo_t *cr,
                                                         PangoLayout *layout)) &
                        abstract_emit_cr_layout_t::emit,
                    this, std::placeholders::_1, std::placeholders::_2));
};
} // namespace uxdevice

namespace uxdevice {
class visitor_base_t {
public:
  visitor_base_t() {}
  virtual ~visitor_base_t() {}
};
} // namespace uxdevice

// abstract labels class as copy and emit local unit memory
namespace uxdevice {
class visitor_textual_render_t : virtual public visitor_base_t {
public:
  visitor_textual_render_t() {}
  virtual ~visitor_textual_render_t() {}
};
} // namespace uxdevice

namespace uxdevice {
class visitor_image_block_t : virtual public visitor_base_t {
public:
  visitor_image_block_t() {}
  virtual ~visitor_image_block_t() {}
};
} // namespace uxdevice

/**
 * \class
 * \tparam ...
 * \brief
 */
namespace uxdevice {
template <typename... Args> class visitor_interfaces_t : public Args... {
public:
  std::unordered_map<std::type_index, visitor_interface_t> visitors = {};
  visitor_interfaces_t() {}
  visitor_interfaces_t(Args &... args) {
    ((void)add_visitor_fn(std::forward<Args>(args)), ...);
  }

  template <typename T> void add_visitor_fn(T &obj_fn) {
    visitors[std::type_index(typeid(T))] = obj_fn.fn_interface;
  }
};
} // namespace uxdevice
