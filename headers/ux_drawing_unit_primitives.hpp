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
\file ux_drawing_primitives.hpp
\date 9/23/20
\version 1.0
\brief
*/

/**
 \class
 \brief
 */
namespace uxdevice {
using function_object_t = class function_object_t
    : public storage_emitter_t<
          function_object_t, cairo_function_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr) { value(cr); }
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::function_object_t);

/**
 \internal
 \class arc_storage_t
 \brief

 \details


 */
namespace uxdevice {
class arc_storage_t : virtual public hash_members_t {
public:
  arc_storage_t() {}
  arc_storage_t(double _xc, double _yc, double _radius, double _angle1,
                double _angle2)
      : xc(_xc), yc(_yc), radius(_radius), angle1(_angle1), angle2(_angle2) {}
  virtual ~arc_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(arc_storage_t)), xc, yc,
                 radius, angle1, angle2);
    return __value;
  }

  double xc = {};
  double yc = {};
  double radius = {};
  double angle1 = {};
  double angle2 = {};
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::arc_storage_t);

/**
 \internal
 \class negative_arc_storage_t
 \brief

 \details


 */
namespace uxdevice {
class negative_arc_storage_t : virtual public hash_members_t {
public:
  negative_arc_storage_t() {}
  negative_arc_storage_t(double _xc, double _yc, double _radius, double _angle1,
                         double _angle2)
      : xc(_xc), yc(_yc), radius(_radius), angle1(_angle1), angle2(_angle2) {}

  virtual ~negative_arc_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(negative_arc_storage_t)), xc,
                 yc, radius, angle1, angle2);
    return __value;
  }

  double xc = {};
  double yc = {};
  double radius = {};
  double angle1 = {};
  double angle2 = {};
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::negative_arc_storage_t);

/**
 \internal
 \class rectangle_storage_t
 \brief

 \details


 */
namespace uxdevice {
class rectangle_storage_t : virtual public hash_members_t {
public:
  rectangle_storage_t() {}
  rectangle_storage_t(double _x, double _y, double _width, double _height)
      : x(_x), y(_y), width(_width), height(_height) {}
  virtual ~rectangle_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(rectangle_storage_t)), x, y,
                 width, height);
    return __value;
  }

  double x = {};
  double y = {};
  double width = {};
  double height = {};
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::rectangle_storage_t);

/**
 \internal
 \class curve_storage_t
 \brief

 \details


 */
namespace uxdevice {
class curve_storage_t : virtual public hash_members_t {
public:
  curve_storage_t() {}
  curve_storage_t(double _x1, double _y1, double _x2, double _y2, double _x3,
                  double _y3)
      : x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3) {}
  virtual ~curve_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(curve_storage_t)), x1, y1, x2,
                 y2, x3, y3);
    return __value;
  }

  double x1 = {};
  double y1 = {};
  double x2 = {};
  double y2 = {};
  double x3 = {};
  double y3 = {};
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::curve_storage_t);

/**
 \internal
 \class line_storage_t
 \brief

 \details


 */
namespace uxdevice {
class line_storage_t : virtual public hash_members_t {
public:
  line_storage_t() {}
  line_storage_t(double _x, double _y) : x(_x), y(_y) {}
  virtual ~line_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(line_storage_t)), x, y);
    return __value;
  }

  double x = {};
  double y = {};
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_storage_t);

/**
 \internal
 \class
 stroke_fill_path_storage_t
 \brief

 \details


 */
namespace uxdevice {
class stroke_fill_path_storage_t : virtual public hash_members_t {
public:
  painter_brush_t fill_brush = {};
  painter_brush_t stroke_brush = {};

  stroke_fill_path_storage_t() {}

  stroke_fill_path_storage_t(const painter_brush_t &f, const painter_brush_t &s)
      : fill_brush(f), stroke_brush(s) {}
  virtual ~stroke_fill_path_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(stroke_fill_path_storage_t)),
                 fill_brush.hash_code(), stroke_brush.hash_code());
    return __value;
  }
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::stroke_fill_path_storage_t);

/**

 \typedef line_dash_storage_t
 \brief storage alias for the
 line dashes array. needed for
 registering the hashing
 function.

 \details


 */
namespace uxdevice {
class line_dash_storage_t : virtual public hash_members_t {
public:
  line_dash_storage_t() {}
  line_dash_storage_t(const std::vector<double> &_value, const double &_offset)
      : value(_value), offset(_offset) {}

  virtual ~line_dash_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(line_dash_storage_t)), offset);
    for (auto n : value)
      hash_combine(__value, n);
    hash_combine(__value, value.size());

    return __value;
  }

  std::vector<double> value = {};
  double offset = {};
};
} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_dash_storage_t);

/**
 \class
 \brief */

namespace uxdevice {
using antialias_t = class antialias_t
    : public storage_emitter_t<
          antialias_t, antialias_options_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_image_block_t, visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;
  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::antialias_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using line_width_t = class line_width_t
    : public storage_emitter_t<
          line_width_t, double,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_image_block_t, visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_width_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using line_cap_t = class line_cap_t
    : public storage_emitter_t<
          line_cap_t, line_cap_options_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_image_block_t, visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_cap_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using line_join_t = class line_join_t
    : public storage_emitter_t<
          line_join_t, line_join_options_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_image_block_t, visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_join_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using miter_limit_t = class miter_limit_t
    : public storage_emitter_t<
          miter_limit_t, double,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_image_block_t, visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::miter_limit_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using line_dashes_t = class line_dashes_t
    : public class_storage_emitter_t<
          line_dashes_t, line_dash_storage_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_image_block_t, visitor_textual_render_t> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_dashes_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using tollerance_t = class tollerance_t
    : public storage_emitter_t<
          tollerance_t, double,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_image_block_t, visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::tollerance_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using graphic_operator_t = class graphic_operator_t
    : public storage_emitter_t<
          graphic_operator_t, graphic_operator_options_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_image_block_t, visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::graphic_operator_t);

/**
 \class
 \brief
 */
// primitives - drawing
// functions
namespace uxdevice {
using arc_t =
    class arc_t : public class_storage_emitter_t<
                      arc_t, arc_storage_t,
                      visitor_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::arc_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using negative_arc_t = class negative_arc_t
    : public class_storage_emitter_t<
          negative_arc_t, negative_arc_storage_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::negative_arc_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using curve_t = class curve_t
    : public class_storage_emitter_t<
          curve_t, curve_storage_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::curve_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using line_t = class line_t
    : public class_storage_emitter_t<
          line_t, line_storage_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using vline_t = class vline_t
    : public storage_emitter_t<
          vline_t, double,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::vline_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using hline_t = class hline_t
    : public storage_emitter_t<
          hline_t, double,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::hline_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using rectangle_t = class rectangle_t
    : public class_storage_emitter_t<
          rectangle_t, rectangle_storage_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::rectangle_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using stroke_path_t = class stroke_path_t
    : public class_storage_emitter_t<
          stroke_path_t, painter_brush_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::stroke_path_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using fill_path_t = class fill_path_t
    : public class_storage_emitter_t<
          fill_path_t, painter_brush_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::fill_path_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using stroke_fill_path_t = class stroke_fill_path_t
    : public class_storage_emitter_t<
          stroke_fill_path_t, stroke_fill_path_storage_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::stroke_fill_path_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using mask_t = class mask_t
    : public class_storage_emitter_t<
          mask_t, painter_brush_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::mask_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using paint_t = class paint_t
    : public storage_emitter_t<
          paint_t, double,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::paint_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using close_path_t = class close_path_t
    : public marker_emitter_t<
          close_path_t,
          visitor_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                               abstract_emit_cr_absolute_t<order_render>>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(cairo_t *cr);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::close_path_t);
