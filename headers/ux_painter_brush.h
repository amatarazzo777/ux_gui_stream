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
  color_stops_provider_t() {}
  virtual ~color_stops_provider_t() {}

  /// @brief constructor accepting unprocessed color_stops sent by api call.
  // color_stops_provider_t(const color_stops_t &_cs) : color_stops(_cs) {}

  /// @brief copy constructor
  color_stops_provider_t(const color_stops_provider_t &other)
    : hash_members_t(other), color_stops(other.color_stops) {}

  /// @brief move constructor
  color_stops_provider_t(color_stops_provider_t &&other) noexcept
    : hash_members_t(other), color_stops(other.color_stops) {}

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

class coordinate_t;

/**
 * @internal
 * @class
 * @brief
 */
class painter_brush_t : public matrix_t {
public:
  /**
   * @internal
   * @class paint_definition_base_t
   * @brief base class for all paint brush types to inherit
   */
  class paint_definition_base_t : virtual public hash_members_t {
  public:
    /**
     * @fn  paint_definition_base_t()
     * @brief
     *
     */
    paint_definition_base_t() {}
    /**
     * @fn  paint_definition_base_t(const std::string&)
     * @brief
     *
     * @param _description
     */
    paint_definition_base_t(const std::string &_description)
      : description(_description) {}
    /**
     * @fn  ~paint_definition_base_t()
     * @brief
     *
     */
    virtual ~paint_definition_base_t() {}

    /// @brief copy constructor
    paint_definition_base_t(const paint_definition_base_t &other)
      : hash_members_t(other), description(other.description),
        matrix(other.matrix), pango_color(other.pango_color),
        is_processed(other.is_processed), is_loaded(other.is_loaded) {}

    /// @brief move constructor
    paint_definition_base_t(paint_definition_base_t &&other) noexcept
      : hash_members_t(other), description(other.description),
        matrix(other.matrix), pango_color(other.pango_color),
        is_processed(other.is_processed), is_loaded(other.is_loaded) {}

    /// @brief copy assignment operator
    paint_definition_base_t &operator=(const paint_definition_base_t &other);

    /// @brief move assignment operator
    paint_definition_base_t &
    operator=(paint_definition_base_t &&other) noexcept;

    std::size_t hash_code(void) const noexcept;

    std::string description = {};
    matrix_t matrix = {};
    PangoColor pango_color = {0, 0, 0};
    bool is_processed = false;
    bool is_loaded = false;
  };

  /**
   * @internal
   * @class
   * @brief
   */
  class descriptive_definition_t : public paint_definition_base_t {
  public:
    /// @brief default constructor
    descriptive_definition_t() {}

    /// @brief string
    descriptive_definition_t(const std::string &_description)
      : paint_definition_base_t(_description) {}

    /// @brief copy constructor
    descriptive_definition_t(const descriptive_definition_t &other)
      : paint_definition_base_t(other) {}

    /// @brief move  constructor
    descriptive_definition_t(descriptive_definition_t &&other) noexcept
      : paint_definition_base_t(other) {}

    /// @brief de-constructor
    virtual ~descriptive_definition_t() {}

    /// @brief copy assignment
    descriptive_definition_t &operator=(const descriptive_definition_t &other);

    /// @brief move assignment
    descriptive_definition_t &
    operator=(descriptive_definition_t &&other) noexcept;

    std::size_t hash_code(void) const noexcept;
  };

  /**
   * @internal
   * @class
   * @brief
   */
  class color_definition_t
    : public paint_definition_base_t,
      public accepted_interfaces_t<abstract_emit_cr_t<order_render_option>>,
      public visitor_targets_t<all_rendering_bits> {
  public:
    /**
     * @fn  color_definition_t()
     * @brief default constructor
     *
     */
    color_definition_t() {}
    /**
     * @fn  color_definition_t(const std::string&, double, double, double,
     * double)
     * @brief
     *
     * @param _description
     * @param _r
     * @param _g
     * @param _b
     * @param _a
     */
    color_definition_t(const std::string &_description, double _r, double _g,
                       double _b, double _a)
      : paint_definition_base_t(_description), r(_r), g(_g), b(_b), a(_a) {
      is_loaded = true;
    }

    /**
     * @fn  color_definition_t(const std::string&)
     * @brief
     *
     * @param _description
     */
    color_definition_t(const std::string &_description)
      : color_definition_t(_description, 1.0) {}

    color_definition_t(const std::string &_description, double _a);
    /**
     * @fn  color_definition_t(const color_definition_t&)
     * @brief
     *
     * @param other
     */
    color_definition_t(const color_definition_t &other)
      : paint_definition_base_t(other), r(other.r), g(other.g), b(other.b),
        a(other.a) {}

    /**
     * @fn  color_definition_t(color_definition_t&&)
     * @brief
     *
     * @param other
     */
    color_definition_t(color_definition_t &&other) noexcept
      : paint_definition_base_t(other), r(std::move(other.r)),
        g(std::move(other.g)), b(std::move(other.b)), a(std::move(other.a)) {}

    bool is_color_description(void);
    color_definition_t &operator=(const color_definition_t &other);
    color_definition_t &operator=(color_definition_t &&other) noexcept;

    virtual ~color_definition_t() {}

    virtual void emit(cairo_t *cr);
    std::size_t hash_code(void) const noexcept;

    double r = {};
    double g = {};
    double b = {};
    double a = 1.0;
  };

  /**
   * @internal
   * @class
   * @brief
   */
  class linear_gradient_definition_t
    : private color_stops_provider_t,
      public paint_definition_base_t,
      public accepted_interfaces_t<abstract_emit_cr_t<order_render_option>,
                                   abstract_emit_cr_a_t<order_render_option>>,
      public visitor_targets_t<all_rendering_bits> {
  public:
    /**
     * @fn  linear_gradient_definition_t()
     * @brief default constructor
     *
     */
    linear_gradient_definition_t() {}

    /**
     * @fn  linear_gradient_definition_t(const std::string&)
     * @brief creation from a description
     *
     * @param _description
     */
    linear_gradient_definition_t(const std::string &_description)
      : paint_definition_base_t(_description) {}

    /**
     * @fn  linear_gradient_definition_t(const std::string&, double, double,
     * double, double, const color_stops_t&, filter_options_t, extend_options_t)
     * @brief formal parameters in binary entirety
     *
     * @param _description
     * @param _x0
     * @param _y0
     * @param _x1
     * @param _y1
     * @param _cs
     * @param _filter
     * @param _extend
     */
    linear_gradient_definition_t(const std::string &_description, double _x0,
                                 double _y0, double _x1, double _y1,
                                 const color_stops_t &_cs,
                                 filter_options_t _filter,
                                 extend_options_t _extend)
      : paint_definition_base_t(_description), x0(_x0), y0(_y0), x1(_x1),
        y1(_y1), filter(_filter), extend(_extend) {
      create();
    }

    /// @virtual destructor
    virtual ~linear_gradient_definition_t();

    /// @brief copy constructor
    linear_gradient_definition_t(const linear_gradient_definition_t &other)
      : color_stops_provider_t(other), paint_definition_base_t(other),
        x0(other.x0), y0(other.y0), x1(other.x1), y1(other.y1),
        filter(other.filter), extend(other.extend),
        pattern(cairo_pattern_reference(other.pattern)) {}

    /// @brief move constructor
    linear_gradient_definition_t(linear_gradient_definition_t &&other) noexcept
      : color_stops_provider_t(other), paint_definition_base_t(other),
        x0(other.x0), y0(other.y0), x1(other.x1), y1(other.y1),
        filter(other.filter), extend(other.extend), pattern(other.pattern) {}

    /// @brief copy assignment
    linear_gradient_definition_t &
    operator=(const linear_gradient_definition_t &other);

    /// @brief move assignment
    linear_gradient_definition_t &
    operator=(linear_gradient_definition_t &&other) noexcept;

    /// @brief interface routines
    bool is_linear_gradient_description(void);
    void create(void);
    virtual void emit(cairo_t *cr);
    virtual void emit(cairo_t *cr, coordinate_t *a);
    std::size_t hash_code(void) const noexcept;

    /// @brief object instance variables
    double x0 = {};
    double y0 = {};
    double x1 = {};
    double y1 = {};

    filter_options_t filter = {};
    extend_options_t extend = {};
    cairo_pattern_t *pattern = {};
  };

  /**
   * @internal
   * @class radial_gradient_definition_t
   * @brief
   *
   */
  class radial_gradient_definition_t
    : public paint_definition_base_t,
      private color_stops_provider_t,
      public accepted_interfaces_t<abstract_emit_cr_t<order_render_option>,
                                   abstract_emit_cr_a_t<order_render_option>>,
      public visitor_targets_t<all_rendering_bits> {
  public:
    /**
     * @fn  radial_gradient_definition_t()
     * @brief default constructor
     *
     */
    radial_gradient_definition_t() {}

    /**
     * @fn  radial_gradient_definition_t(const std::string&, double, double,
     * double, double, double, double, const color_stops_t&, filter_options_t,
     * extend_options_t)
     * @brief construction using formal binary parameters in entirety
     *
     * @param _description
     * @param _cx0
     * @param _cy0
     * @param _radius0
     * @param _cx1
     * @param _cy1
     * @param _radius1
     * @param _cs
     * @param _filter
     * @param _extend
     */
    radial_gradient_definition_t(const std::string &_description, double _cx0,
                                 double _cy0, double _radius0, double _cx1,
                                 double _cy1, double _radius1,
                                 const color_stops_t &_cs,
                                 filter_options_t _filter,
                                 extend_options_t _extend)
      : paint_definition_base_t(_description), cx0(_cx0), cy0(_cy0),
        radius0(_radius0), cx1(_cx1), cy1(_cy1), radius1(_radius1),
        color_stops(_cs), filter(_filter), extend(_extend) {}

    /// @brief virtual destructor
    virtual ~radial_gradient_definition_t();

    /// @brief constructor from string
    radial_gradient_definition_t(const std::string &_description)
      : paint_definition_base_t(_description) {}

    /// @brief copy constructor
    radial_gradient_definition_t(const radial_gradient_definition_t &other)
      : paint_definition_base_t(other), color_stops_provider_t(other),
        cx0(other.cx0), cy0(other.cy0), radius0(other.radius0), cx1(other.cx1),
        cy1(other.cy1), radius1(other.radius1), filter(other.filter),
        extend(other.extend), pattern(other.pattern) {}

    /// @brief move constructor
    radial_gradient_definition_t(radial_gradient_definition_t &&other) noexcept
      : paint_definition_base_t(other), color_stops_provider_t(other),
        cx0(other.cx0), cy0(other.cy0), radius0(other.radius0), cx1(other.cx1),
        cy1(other.cy1), radius1(other.radius1), filter(other.filter),
        extend(other.extend), pattern(other.pattern) {}

    /// @brief copy assignment operator
    radial_gradient_definition_t &
    operator=(const radial_gradient_definition_t &other);

    /// @brief move assignment
    radial_gradient_definition_t &
    operator=(radial_gradient_definition_t &&other) noexcept;

    /// @brief object interface
    bool is_radial_gradient(void);
    virtual void create(void);
    virtual void emit(cairo_t *cr);
    virtual void emit(cairo_t *cr, coordinate_t *a);
    std::size_t hash_code(void) const noexcept;

    /// @brief object instance variables
    double cx0 = {};
    double cy0 = {};
    double radius0 = {};
    double cx1 = {};
    double cy1 = {};
    double radius1 = {};
    color_stops_t color_stops = {};
    filter_options_t filter = {};
    extend_options_t extend = {};
    cairo_pattern_t *pattern = {};
  };

  /**
   * @internal
   * @class
   * @brief
   */

  class image_block_pattern_source_definition_t
    : public paint_definition_base_t,
      public accepted_interfaces_t<abstract_emit_cr_t<order_render_option>,
                                   abstract_emit_cr_a_t<order_render_option>>,
      public visitor_targets_t<all_rendering_bits> {
  public:
    /**
     * @fn  image_block_pattern_source_definition_t()
     * @brief default constructor
     *
     */
    image_block_pattern_source_definition_t() {}

    image_block_pattern_source_definition_t(std::string &_description,
                                            double _w, double _h,
                                            extend_options_t _extend,
                                            filter_options_t _filter);

    image_block_pattern_source_definition_t(std::string &_description,
                                            double _w, double _h);

    image_block_pattern_source_definition_t(std::string &_description,
                                            extend_options_t _extend,
                                            filter_options_t _filter);

    virtual ~image_block_pattern_source_definition_t();

    /**
     * @internal
     * @fn  image_block_pattern_source_definition_t(const
     * image_block_pattern_source_definition_t&)
     * @brief copy constructor
     *
     * @param other
     */
    image_block_pattern_source_definition_t(
      const image_block_pattern_source_definition_t &other)
      : paint_definition_base_t(other), image_buffer(other.image_buffer),
        pattern(other.pattern), filter(other.filter), extend(other.extend) {}

    /**
     * @internal
     * @fn
     * image_block_pattern_source_definition_t(image_block_pattern_source_definition_t&&)
     * @brief
     *
     * @param other
     */
    image_block_pattern_source_definition_t(
      image_block_pattern_source_definition_t &&other) noexcept
      : paint_definition_base_t(other), image_buffer(other.image_buffer),
        pattern(other.pattern), filter(other.filter), extend(other.extend) {}

    /// @brief copy assignment
    image_block_pattern_source_definition_t &
    operator=(const image_block_pattern_source_definition_t &other);

    /// @brief move assignment
    image_block_pattern_source_definition_t &
    operator=(image_block_pattern_source_definition_t &&other) noexcept;

    /// @brief interface
    virtual void emit(cairo_t *cr);
    virtual void emit(cairo_t *cr, coordinate_t *a);
    std::size_t hash_code(void) const noexcept;

    /// @brief instance variables.
    draw_buffer_t image_buffer = {};
    cairo_pattern_t *pattern = {};
    filter_options_t filter = {};
    extend_options_t extend = {};
  };

  /**
   * @internal
   * @typedef
   * @brief holds the paint types in one storage container
   */
  typedef std::variant<std::monostate, descriptive_definition_t,
                       color_definition_t, linear_gradient_definition_t,
                       radial_gradient_definition_t,
                       image_block_pattern_source_definition_t>
    data_storage_t;

  /**
   * @internal
   * @fn painter_brush_t
   * @brief default constructor
   */
  painter_brush_t() {}

  /**
   * @internal
   * @fn
   * @brief color given as a uint32 value
   */
  painter_brush_t(u_int32_t c);
  /**
   * @internal
   * @fn
   * @brief color given as r,g,b
   */
  painter_brush_t(double r, double g, double b)
    : data_storage(color_definition_t{"RGB", r, g, b, 1.0}) {}

  /**
   * @internal
   * @fn
   * @brief color given as r,g,b,a
   */
  painter_brush_t(double r, double g, double b, double a)
    : data_storage(color_definition_t{"RGBA", r, g, b, a}) {}

  /**
   * @internal
   * @fn
   * @brief color given as a description
   */
  painter_brush_t(const std::string_view n)
    : data_storage(descriptive_definition_t{std::string(n)}) {
    create();
  }

  /**
   * @internal
   * @fn
   * @brief color given as a description
   */
  painter_brush_t(std::string &n) : data_storage(descriptive_definition_t{n}) {
    create();
  }

  /**
   * @internal
   * @fn
   * @brief color given as a description
   */
  painter_brush_t(std::string &n, double width, double height)
    : data_storage(image_block_pattern_source_definition_t{n, width, height}) {}

  /**
   * @internal
   * @fn
   * @brief color given as a linear gradient
   */
  painter_brush_t(double x0, double y0, double x1, double y1,
                  const color_stops_t &cs)
    : data_storage(linear_gradient_definition_t{"linear_gradient", x0, y0, x1,
                                                y1, cs, filter_options_t::fast,
                                                extend_options_t::repeat}) {}

  /**
   * @internal
   * @fn
   * @brief color given as a radial gradient
   */
  painter_brush_t(double cx0, double cy0, double radius0, double cx1,
                  double cy1, double radius1, const color_stops_t &cs)
    : data_storage(radial_gradient_definition_t{
        "radial_gradient", cx0, cy0, radius0, cx1, cy1, radius1, cs,
        filter_options_t::fast, extend_options_t::off}) {}

  virtual ~painter_brush_t() {}

  /// @brief copy constructor
  painter_brush_t(const painter_brush_t &other)
    : matrix_t(other), data_storage(other.data_storage) {}

  /// @brief move constructor
  painter_brush_t(painter_brush_t &&other)
    : matrix_t(other), data_storage(std::move(other.data_storage)) {}

  /// @brief copy assignment
  painter_brush_t &operator=(const painter_brush_t &other);

  /// @brief move assignment
  painter_brush_t &operator=(painter_brush_t &&other) noexcept;

  virtual void emit(cairo_t *cr);
  virtual void emit(cairo_t *cr, coordinate_t *coord);
  bool is_valid(void);

private:
  bool create(void);

  bool is_linear_gradient(const std::string &s);
  bool is_radial_gradient(const std::string &s);
  bool patch(const std::string &s);

public:
  std::size_t hash_code(void) const noexcept;

  data_storage_t data_storage = {};
  typedef std::function<data_storage_t(const std::string &)> dt_fn_storage_t;
  typedef std::list<dt_fn_storage_t> paint_factories_t;

  static paint_factories_t paint_factories;
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::color_stop_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(
  uxdevice::painter_brush_t::paint_definition_base_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(
  uxdevice::painter_brush_t::descriptive_definition_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(
  uxdevice::painter_brush_t::color_definition_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(
  uxdevice::painter_brush_t::linear_gradient_definition_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(
  uxdevice::painter_brush_t::radial_gradient_definition_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(
  uxdevice::painter_brush_t::image_block_pattern_source_definition_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::painter_brush_t)
