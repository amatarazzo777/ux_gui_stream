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
 * @file ux_painter_brush.cpp
 * @date 5/12/20
 * @version 1.0
 * @details Class provides the painting object interface which invokes the
 * appropriate cairo API. The notable virtual method is the emit function which
 * applies the cairo text_color_t setting.
 */

#include <ux_base.h>
#include <ux_hash.h>
#include <ux_system_error.h>
#include <ux_enums.h>
#include <ux_matrix.h>
#include <ux_visitor_interface.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>
#include <ux_pipeline_memory.h>
#include <ux_display_visual.h>

#include <ux_display_context.h>
#include <ux_display_unit_base.h>

#include <ux_coordinate.h>

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
/**
 * @fn color_stops_provider_t operator =&(const color_stops_provider_t&)
 * @brief copy assignment
 *
 * @param other
 * @return
 */
uxdevice::color_stops_provider_t &uxdevice::color_stops_provider_t::operator=(
  const color_stops_provider_t &other) {
  hash_members_t::operator=(other);
  color_stops = other.color_stops;
  return *this;
}

/**
 * @fn color_stops_provider_t operator =&(const color_stops_provider_t&&)
 * @brief move assignment
 *
 * @param other
 * @return
 */
uxdevice::color_stops_provider_t &uxdevice::color_stops_provider_t::operator=(
  color_stops_provider_t &&other) noexcept {
  hash_members_t::operator=(other);
  color_stops = std::move(other.color_stops);
  return *this;
}

/**
 * @internal
 * @fn void resolve(cairo_pattern_t*)
 * @brief  so this logic is used in duel
 * form. When gradients may be named as a string, or provided in complete
 * API form. the logic below fills in the offset values automatically
 * distributing equally across the noted offset. offsets are provided from 0
 * - 1 and name the point within the line.
 *
 * @param pattern
 */
void uxdevice::color_stops_provider_t::resolve_color_stops(
  cairo_pattern_t *pattern) {
  if (!pattern || color_stops.size() == 0)
    return;

  bool bDone = false;
  bool bEdgeEnd = false;

  // first one, if auto offset set to
  //   0 - the beginning of the color stops
  color_stops_iterator_t it = color_stops.begin();
  if (it->bAutoOffset) {
    it->bAutoOffset = false;
    it->offset = 0;
  }
  double dOffset = it->offset;

  while (!bDone) {

    // find first color stop with a defined offset.
    color_stops_iterator_t it2 = find_if(
      it + 1, color_stops.end(), [](auto const &o) { return !o.bAutoOffset; });

    // not found, the last item in color stops did not have a value,
    // assign it 1.0
    if (it2 == color_stops.end()) {
      bEdgeEnd = true;
      bDone = true;
      // very last one has a setting
    } else if (it2 == color_stops.end() - 1) {
      bDone = true;
    }

    // distribute offsets equally across range
    int ncolor_stops_t = std::distance(it, it2);
    if (bEdgeEnd)
      ncolor_stops_t--;

    if (ncolor_stops_t > 0) {
      double incr = 0;
      if (bEdgeEnd) {
        incr = (1 - it->offset) / ncolor_stops_t;
      } else {
        incr = (it2->offset - it->offset) / ncolor_stops_t;
        ncolor_stops_t--;
      }

      dOffset = it->offset;
      while (ncolor_stops_t) {
        it++;
        dOffset += incr;
        it->offset = dOffset;
        it->bAutoOffset = false;
        ncolor_stops_t--;
      }
    }
    // forward to next range
    it = it2;
  }
  // add the color stops
  std::for_each(color_stops.begin(), color_stops.end(), [=](auto &n) {
    if (n.bRGBA)
      cairo_pattern_add_color_stop_rgba(pattern, n.offset, n.r, n.g, n.b, n.a);
    else
      cairo_pattern_add_color_stop_rgb(pattern, n.offset, n.r, n.g, n.b);
  });
}

/**
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::color_stops_provider_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  for (auto n : color_stops)
    hash_combine(__value, n.hash_code());

  return __value;
}

/**
 * @internal
 * @fn
 * @brief color given as a uint32 value
 */
uxdevice::painter_brush_t::painter_brush_t(u_int32_t c) {

  auto r = static_cast<u_int8_t>(c >> 16) / 255.0;
  auto g = static_cast<u_int8_t>(c >> 8) / 255.0;
  auto b = static_cast<u_int8_t>(c) / 255.0;
  auto a = 1.0;

  data_storage = color_definition_t{"u_int32_t RGB", r, g, b, a};
}

/**
 * @fn painter_brush_t operator =&(const painter_brush_t&)
 * @brief copy assignment
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t &
uxdevice::painter_brush_t::operator=(const painter_brush_t &other) {
  matrix_t::operator=(other);
  data_storage = other.data_storage;
  return *this;
}

/**
 * @fn painter_brush_t operator =&(painter_brush_t&&)
 * @brief move assignment
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t &
uxdevice::painter_brush_t::operator=(painter_brush_t &&other) noexcept {
  data_storage = std::move(other.data_storage);
  matrix_t::operator=(other);
  return *this;
}

/**
 * @internal
 * @brief setup a std::list of paint type creation functions, factories that
 * check if the given string is processed by an object. The creation order
 * is in the same order as it appears below. This list is initialized once at
 * application start. Each of the object constructors is expected to parse the
 * string. If it is applicable, the is_loaded flag is set to true. Otherwise the
 * factory lambda returns std::monostate.
 */
uxdevice::painter_brush_t::paint_factories_t
  uxdevice::painter_brush_t::paint_factories = {

    /**
     * @internal
     * @brief factory lambda for image_block_pattern_source_definition_t given
     * a string. */
    [](auto s) {
      data_storage_t o_ret = {};
      auto o = image_block_pattern_source_definition_t(
        s, extend_options_t::repeat, filter_options_t::fast);
      if (o.is_loaded)
        o_ret = o;

      return o_ret;
    },

    /**
     * @internal
     * @brief factory lambda for linear_gradient_definition_t given a string. */
    [](auto s) {
      data_storage_t o_ret = {};
      auto o = linear_gradient_definition_t(s);
      if (o.is_loaded)
        o_ret = o;

      return o_ret;
    },

    /**
     * @internal
     * @brief factory lambda for radial_gradient_definition_t given a string. */
    [](auto s) {
      data_storage_t o_ret = {};
      auto o = radial_gradient_definition_t(s);
      if (o.is_loaded)
        o_ret = o;

      return o_ret;
    },

    /**
     * @internal
     * @brief factory lambda for color_definition_t given a string. */
    [](auto s) {
      data_storage_t o_ret = {};
      auto o = color_definition_t(s);
      if (o.is_loaded)
        o_ret = o;

      return o_ret;
    }};

/**
 * @internal
 * @fn create
 * @brief The routine handles the creation of the pattern or surface. Patterns
 * can be an image_block_t file, a description of a linear, actual parameters of
 * linear, a description of a radial, the actual radial parameters stored. SVG
 * inline or a base64 data set.
 */
bool uxdevice::painter_brush_t::create(void) {

  // setup visitor function to get member variable "is_processed"
  auto fn_visit_is_processed = [&]() -> bool {
    return std::visit(
      [&](auto &&__v) -> bool {
        using T = std::decay_t<decltype(__v)>;
        if constexpr (std::is_same_v<T, std::monostate>)
          return false;
        else
          return __v.is_processed;
      },
      data_storage);
  };

  // already created?
  if (fn_visit_is_processed())
    return true;

  bool object_created = false;

  // if a description was provided, determine how it should be interpreted
  if (std::holds_alternative<descriptive_definition_t>(data_storage)) {

    auto ddt = std::get<descriptive_definition_t>(data_storage);

    /** @brief iterate the paint types to see which one is applicable. when one
     * is successfully created, the data_storage will no longer hold the
     * std::monostate */
    for (auto n : paint_factories) {

      // try to create
      data_storage = n(ddt.description);

      // success?
      if (!std::holds_alternative<std::monostate>(data_storage)) {
        object_created = true;
        break;
      }
    }
  }

  return object_created;
}

/**
 * @internal
 * @fn emit
 * @param cairo_t *cr
 * @brief The paint is loaded if need be. The paint is set for the passed
 * surface.
 */
void uxdevice::painter_brush_t::emit(cairo_t *cr) {
  std::visit(
    [&](auto &&o) {
      using T = std::decay_t<decltype(o)>;
      if constexpr (std::is_same_v<T, std::monostate> ||
                    std::is_same_v<T, descriptive_definition_t>)
        return;
      else {
        if (!o.is_processed)
          create();
        if (o.is_processed)
          o.emit(cr);
      }
    },
    data_storage);
}

/**
 * @internal
 * @fn emit
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief The paint is loaded if need be. the paint is set for the passed
 * surface.
 */
void uxdevice::painter_brush_t::emit(cairo_t *cr, coordinate_t *a) {

  std::visit(
    [&](auto &&arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, std::monostate> ||
                    std::is_same_v<T, descriptive_definition_t> ||
                    std::is_same_v<T, color_definition_t>)
        return;
      else {
        if (!arg.is_processed)
          create();
        if (arg.is_processed) {
          translate(-a->x, -a->y);
          arg.emit(cr, a);
        }
      }
    },
    data_storage);
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::painter_brush_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(this)));

  hash_combine(__value, std::visit(
                          [](auto &&arg) -> std::size_t {
                            using T = std::decay_t<decltype(arg)>;
                            if constexpr (std::is_same_v<T, std::monostate>)
                              return std::type_index(typeid(T)).hash_code();
                            else
                              return arg.hash_code();
                          },
                          data_storage));

  return __value;
}

/**
 * @fn uxdevice::painter_brush_t::paint_definition_base_t operator =&(const
 * paint_definition_base_t&)
 * @brief copy assignment operator
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::paint_definition_base_t &
uxdevice::painter_brush_t::paint_definition_base_t::operator=(
  const paint_definition_base_t &other) {
  hash_members_t::operator=(other);
  description = other.description;
  matrix = other.matrix;
  pango_color = other.pango_color;
  is_processed = other.is_processed;
  is_loaded = other.is_loaded;
  return *this;
}

/**
 * @fn uxdevice::painter_brush_t::paint_definition_base_t operator
 * =&(paint_definition_base_t&&)
 * @brief move assignment operator
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::paint_definition_base_t &
uxdevice::painter_brush_t::paint_definition_base_t::operator=(
  paint_definition_base_t &&other) noexcept {
  hash_members_t::operator=(other);
  description = std::move(other.description);
  matrix = std::move(other.matrix);
  pango_color = std::move(other.pango_color);
  is_processed = std::move(other.is_processed);
  is_loaded = std::move(other.is_loaded);
  return *this;
}

/**
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::painter_brush_t::paint_definition_base_t::hash_code(
  void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(painter_brush_t)), description,
               pango_color.red, pango_color.green, pango_color.blue, is_loaded);

  return __value;
}

/**
 * @fn uxdevice::painter_brush_t::descriptive_definition_t operator =&(const
 * descriptive_definition_t&)
 * @brief copy assignment
 *
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::descriptive_definition_t &
uxdevice::painter_brush_t::descriptive_definition_t::operator=(
  const descriptive_definition_t &other) {
  paint_definition_base_t::operator=(other);
  return *this;
}

/**
 * @fn uxdevice::painter_brush_t::descriptive_definition_t operator
 * =&(descriptive_definition_t&&)
 * @brief move assignment
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::descriptive_definition_t &
uxdevice::painter_brush_t::descriptive_definition_t::operator=(
  descriptive_definition_t &&other) noexcept {
  paint_definition_base_t::operator=(other);
  return *this;
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::painter_brush_t::descriptive_definition_t::hash_code(
  void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(painter_brush_t)),
               paint_definition_base_t::hash_code());

  return __value;
}

/**
 * @internal
 * @fn  color_definition_t(const std::string&, double)
 * @brief
 *
 * @param _description
 * @param _a
 *
 *
 *
 */
uxdevice::painter_brush_t::color_definition_t::color_definition_t(
  const std::string &_description, double _a)
  : paint_definition_base_t(_description) {
  if (pango_color_parse(&pango_color, _description.data())) {
    r = pango_color.red / 65535.0;
    g = pango_color.green / 65535.0;
    b = pango_color.blue / 65535.0;
    a = _a;
    is_loaded = true;
  }
}

/**
 * @internal
 * @fn color_definition_t operator =&(const color_definition_t&)
 * @brief
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::color_definition_t &
uxdevice::painter_brush_t::color_definition_t::operator=(
  const color_definition_t &other) {
  r = other.r;
  g = other.g;
  b = other.b;
  a = other.a;
  return *this;
}

/**
 * @fn uxdevice::painter_brush_t::color_definition_t operator
 * =&(color_definition_t&&)
 * @brief
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::color_definition_t &
uxdevice::painter_brush_t::color_definition_t::operator=(
  color_definition_t &&other) noexcept {
  r = other.r;
  g = other.g;
  b = other.b;
  a = other.a;
  return *this;
}
/**
 * @internal
 * @fn bool is_color_description(void)
 * @brief
 *
 * @return
 */
bool uxdevice::painter_brush_t::color_definition_t::is_color_description(void) {
  bool bret = false;
  if (pango_color_parse(&pango_color, description.data())) {
    bret = true;
  }
  return bret;
}

/**
 * @internal
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_t::color_definition_t::emit(cairo_t *cr) {
  if (a == 1.0)
    cairo_set_source_rgb(cr, r, g, b);
  else
    cairo_set_source_rgba(cr, r, g, b, a);
}

/**
 * @fn std::size_t hash_code(void) const
 * @brief
 *
 * @return
 */
std::size_t
uxdevice::painter_brush_t::color_definition_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, paint_definition_base_t::hash_code(),
               std::type_index(typeid(this)).hash_code(), r, g, b, a);

  return __value;
}

/**
 * @fn  ~linear_gradient_definition_t()
 * @brief
 *
 */
uxdevice::painter_brush_t::linear_gradient_definition_t::
  ~linear_gradient_definition_t() {
  if (pattern)
    cairo_pattern_destroy(pattern);
}

/**
 * @brief copy assignment
 */
uxdevice::painter_brush_t::linear_gradient_definition_t &
uxdevice::painter_brush_t::linear_gradient_definition_t::operator=(
  const linear_gradient_definition_t &other) {
  color_stops_provider_t::operator=(other);
  paint_definition_base_t::operator=(other);
  x0 = other.x0;
  y0 = other.y0;
  x1 = other.x1;
  y1 = other.y1;
  color_stops = other.color_stops;
  filter = other.filter;
  extend = other.extend;
  pattern = cairo_pattern_reference(other.pattern);
  return *this;
}

/**
 * move assignment
 */
uxdevice::painter_brush_t::linear_gradient_definition_t &
uxdevice::painter_brush_t::linear_gradient_definition_t::operator=(
  linear_gradient_definition_t &&other) noexcept {
  color_stops_provider_t::operator=(other);
  paint_definition_base_t::operator=(other);
  x0 = other.x0;
  y0 = other.y0;
  x1 = other.x1;
  y1 = other.y1;
  color_stops = other.color_stops;
  filter = other.filter;
  extend = other.extend;
  pattern = other.pattern;
  return *this;
}

/**
 * @fn bool is_linear_gradient_description(void)
 * @brief
 *
 * @return
 */
bool uxdevice::painter_brush_t::linear_gradient_definition_t::
  is_linear_gradient_description(void) {
  const std::string_view sLinearPattern = "linear-gradient";
  if (description.compare(0, sLinearPattern.size(), sLinearPattern) != 0)
    return false;

  return true;
}

/**
 * @internal
 * @fn void create(void)
 * @brief
 *
 */
void uxdevice::painter_brush_t::linear_gradient_definition_t::create(void) {
  pattern = cairo_pattern_create_linear(x0, y0, x1, y1);
  resolve_color_stops(pattern);
  cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
  is_processed = true;
  is_loaded = true;
}

/**
 * @internal
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_t::linear_gradient_definition_t::emit(
  cairo_t *cr) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @internal
 * @fn void emit(cairo_t*, coordinate_t*)
 * @brief
 *
 * @param cr
 * @param a
 */
void uxdevice::painter_brush_t::linear_gradient_definition_t::emit(
  cairo_t *cr, coordinate_t *a) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief hash of unit including color stops.
 *
 * @return value std::size_t
 */
std::size_t uxdevice::painter_brush_t::linear_gradient_definition_t::hash_code(
  void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, color_stops_provider_t::hash_code(),
               paint_definition_base_t::hash_code(),
               std::type_index(typeid(this)).hash_code(), x0, y0, x1, y1,
               filter, extend, pattern);

  return __value;
}

/**
 * @fn ~radial_gradient_definition_t
 * @brief destructor
 */
uxdevice::painter_brush_t::radial_gradient_definition_t::
  ~radial_gradient_definition_t() {
  if (pattern)
    cairo_pattern_destroy(pattern);
}

/**
 * @fn operator=
 * @brief copy assignment
 * @param const radial_gradient_definition_t &other
 */
uxdevice::painter_brush_t::radial_gradient_definition_t &
uxdevice::painter_brush_t::radial_gradient_definition_t::operator=(
  const radial_gradient_definition_t &other) {
  color_stops_provider_t::operator=(other);
  cx0 = other.cx0;
  cy0 = other.cy0;
  radius0 = other.radius0;
  cx1 = other.cx1;
  cy1 = other.cy1;
  radius1 = other.radius1;
  color_stops = other.color_stops;
  filter = other.filter;
  extend = other.extend;
  pattern = other.pattern;
  return *this;
}

/**
 * @fn operator=
 * @brief copy assignment
 * @param const radial_gradient_definition_t &other
 */
uxdevice::painter_brush_t::radial_gradient_definition_t &
uxdevice::painter_brush_t::radial_gradient_definition_t::operator=(
  radial_gradient_definition_t &&other) noexcept {
  color_stops_provider_t::operator=(other);
  cx0 = other.cx0;
  cy0 = other.cy0;
  radius0 = other.radius0;
  cx1 = other.cx1;
  cy1 = other.cy1;
  radius1 = other.radius1;
  color_stops = other.color_stops;
  filter = other.filter;
  extend = other.extend;
  pattern = other.pattern;
  return *this;
}

/**
 * @fn bool is_radial_gradient(void)
 * @brief
 *
 * @return
 */
bool uxdevice::painter_brush_t::radial_gradient_definition_t::
  is_radial_gradient(void) {
  const std::string_view sRadialPattern = "radial-gradient";
  if (description.compare(0, sRadialPattern.size(), sRadialPattern) != 0)
    return false;

  return true;
}

/**
 * @fn void create(void)
 * @brief
 *
 */
void uxdevice::painter_brush_t::radial_gradient_definition_t::create(void) {
  pattern = cairo_pattern_create_radial(cx0, cy0, radius0, cx1, cy1, radius1);
  resolve_color_stops(pattern);
  cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
  is_processed = true;
  is_loaded = true;
}

/**
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_t::radial_gradient_definition_t::emit(
  cairo_t *cr) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @fn void emit(cairo_t*, coordinate_t*)
 * @brief
 *
 * @param cr
 * @param a
 */
void uxdevice::painter_brush_t::radial_gradient_definition_t::emit(
  cairo_t *cr, coordinate_t *a) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * radial_gradient.@fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::painter_brush_t::radial_gradient_definition_t::hash_code(
  void) const noexcept {
  std::size_t __value = {};

  hash_combine(__value, color_stops_provider_t::hash_code(),
               paint_definition_base_t::hash_code(),
               std::type_index(typeid(this)).hash_code(), cx0, cy0, radius0,
               cx1, cy1, radius1, filter, extend, pattern);

  return __value;
}

/**
 * @fn  image_block_pattern_source_definition_t(std::string&, double,
 * double, extend_options_t, filter_options_t)
 * @brief
 *
 * @param _description
 * @param _w
 * @param _h
 * @param _extend
 * @param _filter
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
  image_block_pattern_source_definition_t(std::string &_description, double _w,
                                          double _h, extend_options_t _extend,
                                          filter_options_t _filter)
  : paint_definition_base_t(_description), image_buffer(_description, _w, _h) {
  pattern = cairo_pattern_create_for_surface(image_buffer.rendered);
  cairo_pattern_set_extend(pattern, static_cast<cairo_extend_t>(_extend));
  cairo_pattern_set_filter(pattern, static_cast<cairo_filter_t>(_filter));
}

/**
 * @fn  image_block_pattern_source_definition_t(std::string&, double,
 * double, extend_options_t, filter_options_t)
 * @brief
 *
 * @param _description
 * @param _w
 * @param _h
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
  image_block_pattern_source_definition_t(std::string &_description, double _w,
                                          double _h)
  : paint_definition_base_t(_description), image_buffer(_description, _w, _h) {
  pattern = cairo_pattern_create_for_surface(image_buffer.rendered);
  cairo_pattern_set_extend(
    pattern, static_cast<cairo_extend_t>(extend_options_t::repeat));
  cairo_pattern_set_filter(pattern,
                           static_cast<cairo_filter_t>(filter_options_t::fast));
}

/**
 * @fn  image_block_pattern_source_definition_t(std::string&,
 * extend_options_t, filter_options_t)
 * @brief
 *
 * @param _description
 * @param _extend
 * @param _filter
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
  image_block_pattern_source_definition_t(std::string &_description,
                                          extend_options_t _extend,
                                          filter_options_t _filter)
  : paint_definition_base_t(_description), image_buffer(_description) {
  pattern = cairo_pattern_create_for_surface(image_buffer.rendered);
  cairo_pattern_set_extend(pattern, static_cast<cairo_extend_t>(_extend));
  cairo_pattern_set_filter(pattern, static_cast<cairo_filter_t>(_filter));
}

/**
 * @internal
 * @fn  ~image_block_pattern_source_definition_t()
 * @brief
 *
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::
  ~image_block_pattern_source_definition_t() {
  if (pattern)
    cairo_pattern_destroy(pattern);
}

/**
 * @internal
 * @fn image_block_pattern_source_definition_t operator =&(const
 * image_block_pattern_source_definition_t&)
 * @brief
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t &
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::operator=(
  const image_block_pattern_source_definition_t &other) {
  image_buffer = other.image_buffer;
  pattern = other.pattern;
  filter = other.filter;
  extend = other.extend;
  return *this;
}

/**
 * @fn image_block_pattern_source_definition_t operator =&(const
 * image_block_pattern_source_definition_t&&)
 * @brief move assignment
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t::image_block_pattern_source_definition_t &
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::operator=(
  image_block_pattern_source_definition_t &&other) noexcept {
  image_buffer = other.image_buffer;
  pattern = other.pattern;
  filter = other.filter;
  extend = other.extend;
  return *this;
}

/**
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_t::image_block_pattern_source_definition_t::emit(
  cairo_t *cr) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @fn void emit(cairo_t*, coordinate_t*)
 * @brief
 *
 * @param cr
 * @param a
 */
void uxdevice::painter_brush_t::image_block_pattern_source_definition_t::emit(
  cairo_t *cr, coordinate_t *a) {
  cairo_pattern_set_matrix(pattern, &matrix._matrix);
  cairo_set_source(cr, pattern);
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t
uxdevice::painter_brush_t::image_block_pattern_source_definition_t::hash_code(
  void) const noexcept {
  std::size_t __value = {};

  hash_combine(__value, paint_definition_base_t::hash_code(),
               std::type_index(typeid(this)), image_buffer.hash_code(), pattern,
               filter, extend);

  return __value;
}
