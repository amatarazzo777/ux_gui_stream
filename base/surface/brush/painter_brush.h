#pragma once

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
 * @file painter_brush.hpp
 * @date 9/7/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {

class coordinate_t;
class hash_members_t;

/**
 * @internal
 * @class
 * @brief
 */
class painter_brush_t : public matrix_t {
public:
  class paint_definition_base_t;
  class descriptive_definition_t;
  class color_definition_t;
  class linear_gradient_definition_t;
  class radial_gradient_definition_t;


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
    image_block_pattern_source_definition_t();

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

    image_block_pattern_source_definition_t(
        const image_block_pattern_source_definition_t &other);
    image_block_pattern_source_definition_t(
        image_block_pattern_source_definition_t &&other) noexcept;

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
      : data_storage(
            image_block_pattern_source_definition_t{n, width, height}) {}

  /**
   * @internal
   * @fn
   * @brief color given as a linear gradient
   */
  painter_brush_t(double x0, double y0, double x1, double y1,
                  const color_stops_t &cs)
      : data_storage(linear_gradient_definition_t{
            "linear_gradient", x0, y0, x1, y1, cs, filter_options_t::fast,
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
