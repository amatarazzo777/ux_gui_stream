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
@file ux_drawing_primitives.hpp
@date 9/23/20
@version 1.0
@brief
*/

namespace uxdevice {
/**
 @class
 @brief
 */
class function_object_t
    : public storage_emitter_t<
          function_object_t, cairo_function_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr) { value(cr); }
};

/**
 @internal
 @class arc_storage_t
 @brief

 @details


 */
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

/**
 @internal
 @class negative_arc_storage_t
 @brief

 @details


 */
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

/**
 @internal
 @class rectangle_storage_t
 @brief

 @details


 */
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

/**
 @internal
 @class curve_storage_t
 @brief

 @details


 */
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

/**
 @internal
 @class line_storage_t
 @brief

 @details


 */
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

/**
 @internal
 @class
 stroke_fill_path_storage_t
 @brief

 @details


 */
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

/**

 \typedef line_dash_storage_t
 @brief storage alias for the
 line dashes array. needed for
 registering the hashing
 function.

 @details


 */
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

/**
 @class
 @brief */
class antialias_t
    : public storage_emitter_t<
          antialias_t, antialias_options_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits, image_block_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;
  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class line_width_t
    : public storage_emitter_t<
          line_width_t, double,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_path_bits, image_block_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class line_cap_t
    : public storage_emitter_t<
          line_cap_t, line_cap_options_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_path_bits, image_block_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class line_join_t
    : public storage_emitter_t<
          line_join_t, line_join_options_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_path_bits, image_block_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class miter_limit_t
    : public storage_emitter_t<
          miter_limit_t, double,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_path_bits, image_block_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class line_dashes_t
    : public class_storage_emitter_t<
          line_dashes_t, line_dash_storage_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_path_bits, image_block_bits>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class tollerance_t
    : public storage_emitter_t<
          tollerance_t, double,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_path_bits, image_block_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class graphic_operator_t
    : public storage_emitter_t<
          graphic_operator_t, graphic_operator_options_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits, image_block_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
// primitives - drawing
// functions
class arc_t : public class_storage_emitter_t<
                  arc_t, arc_storage_t,
                  accepted_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class negative_arc_t
    : public class_storage_emitter_t<
          negative_arc_t, negative_arc_storage_t,
          accepted_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                                abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class curve_t
    : public class_storage_emitter_t<
          curve_t, curve_storage_t,
          accepted_interfaces_t<abstract_emit_context_t<order_render>,
                                abstract_emit_cr_relative_t<order_render>,
                                abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;
  void emit(display_context_t *context) {
    if (context->pipeline_memory_access<relative_coordinate_t>())
      emit_relative(context->cr);
    else
      emit_absolute(context->cr);
  }
  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
};

/**
 @class
 @brief
 */
class line_t
    : public class_storage_emitter_t<
          line_t, line_storage_t,
          accepted_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                                abstract_emit_cr_absolute_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;
  void emit(display_context_t *context) {
    if (context->pipeline_memory_access<relative_coordinate_t>())
      emit_relative(context->cr);
    else
      emit_absolute(context->cr);
  }
  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
};

/**
 @class
 @brief
 */
class vline_t
    : public storage_emitter_t<
          vline_t, double,
          accepted_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                                abstract_emit_cr_absolute_t<order_render>>> {
public:
  using storage_emitter_t::storage_emitter_t;
  void emit(display_context_t *context) {
    if (context->pipeline_memory_access<relative_coordinate_t>())
      emit_relative(context->cr);
    else
      emit_absolute(context->cr);
  }
  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
};

/**
 @class
 @brief
 */
class hline_t
    : public storage_emitter_t<
          hline_t, double,
          accepted_interfaces_t<abstract_emit_cr_relative_t<order_render>,
                                abstract_emit_cr_absolute_t<order_render>>> {
public:
  using storage_emitter_t::storage_emitter_t;
  void emit(display_context_t *context) {
    if (context->pipeline_memory_access<relative_coordinate_t>())
      emit_relative(context->cr);
    else
      emit_absolute(context->cr);
  }
  void emit_relative(cairo_t *cr);
  void emit_absolute(cairo_t *cr);
};

/**
 @class
 @brief
 */
class rectangle_t
    : public class_storage_emitter_t<
          rectangle_t, rectangle_storage_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class stroke_path_t
    : public class_storage_emitter_t<
          stroke_path_t, painter_brush_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class fill_path_t
    : public class_storage_emitter_t<
          fill_path_t, painter_brush_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class stroke_fill_path_t
    : public class_storage_emitter_t<
          stroke_fill_path_t, stroke_fill_path_storage_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
using mask_t = class mask_t
    : public class_storage_emitter_t<
          mask_t, painter_brush_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits, image_block_bits>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class paint_t
    : public storage_emitter_t<
          paint_t, double,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits, image_block_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(cairo_t *cr);
};

/**
 @class
 @brief
 */
class close_path_t
    : public marker_emitter_t<
          close_path_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(cairo_t *cr);
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::function_object_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::arc_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::negative_arc_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::rectangle_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::curve_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::stroke_fill_path_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_dash_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::antialias_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_width_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_cap_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_join_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::miter_limit_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_dashes_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::tollerance_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::graphic_operator_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::arc_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::negative_arc_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::curve_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::line_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::vline_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::hline_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::rectangle_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::stroke_path_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::fill_path_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::stroke_fill_path_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::mask_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::paint_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::close_path_t);
