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
 @file ux_text_units.h
 @date 9/7/20
 @version 1.0
 @brief
 */

namespace uxdevice {

/**
 * @internal
 * @class text_font_data_storage
 * @brief storage for fonts.@
 * @details
 */
class text_font_storage_t : virtual public hash_members_t {
public:
  // these become public
  // members of the base
  // class.
  text_font_storage_t() : description{}, font_ptr(nullptr) {}
  text_font_storage_t(const std::string &_description)
      : description(_description), font_ptr(nullptr) {}

  /// @brief move constructor
  text_font_storage_t(text_font_storage_t &&other) noexcept
      : description(other.description), font_ptr(other.font_ptr) {}

  /// @brief copy constructor
  text_font_storage_t(const text_font_storage_t &other)
      : description(other.description), font_ptr(nullptr) {}

  virtual ~text_font_storage_t() {
    if (font_ptr)
      pango_font_description_free(font_ptr);
  }

  text_font_storage_t &operator=(const text_font_storage_t &&other) noexcept {
    description = other.description;
    return *this;
  }
  text_font_storage_t &operator=(const text_font_storage_t &other) {
    description = other.description;
    return *this;
  }

  text_font_storage_t &operator=(const std::string &_desc) {
    description = _desc;
    return *this;
  }

  /// @brief move assignment
  text_font_storage_t &operator=(const std::string &&_desc) noexcept {
    description = _desc;
    return *this;
  }

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(text_font_storage_t)),
                 description);

    return __value;
  }

  std::string description = {};
  PangoFontDescription *font_ptr = {};
};

/**
 * @class text_font_t
 * @brief
 */
class text_font_t
    : public class_storage_emitter_t<
          text_font_t, text_font_storage_t,
          accepted_interfaces_t<abstract_emit_layout_t<order_render_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;
  void emit(PangoLayout *layout);
};

/**
 * @class text_render_normal_t
 * @brief
 */
class text_render_normal_t
    : public marker_emitter_t<
          text_render_normal_t,
          accepted_interfaces_t<abstract_emit_context_t<order_init>,
                                abstract_emit_cr_layout_t<order_render>>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
  void emit(cairo_t *cr, PangoLayout *layout);
};

/**
 * @class text_render_path_t
 * @brief
 */
class text_render_path_t
    : public marker_emitter_t<
          text_render_path_t,
          accepted_interfaces_t<abstract_emit_context_t<order_init>,
                                abstract_emit_cr_layout_t<order_render>>> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
  void emit(cairo_t *cr, PangoLayout *layout);
};

/**
 @class
 @brief
 */
class text_color_t
    : public painter_brush_emitter_t<
          text_color_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render_option>,
                                abstract_emit_cr_a_t<order_render_option>>,
          visitor_targets_t<textual_render_normal_bits>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;
  void emit(cairo_t *cr) { painter_brush_emitter_t::emit(cr); }
  void emit(cairo_t *cr, coordinate_t *a) {
    painter_brush_emitter_t::emit(cr, a);
  }
};

/**
 * @class text_outline_t
 * @brief
 */
class text_outline_t
    : public painter_brush_emitter_t<
          text_outline_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>,
                                abstract_emit_cr_a_t<order_render>>,
          visitor_targets_t<textual_render_path_bits>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;

  // since class implements the painter_brush_emitter_t, specializations may
  // call the base constructor, that supply the painter_brush_emitter interface
  // that is inherited. The emitter functions should call the
  // base class members.
  void emit(cairo_t *cr);
  void emit(cairo_t *cr, coordinate_t *a);

  // local property that returns the same object. Continuation syntax for inline
  // options.
  text_outline_t &width(double __width) {
    _width = __width;
    return *this;
  }
  double _width = 1;
};

/**
 * @class text_fill_t
 * @brief
 */
class text_fill_t
    : public painter_brush_emitter_t<
          text_fill_t,
          accepted_interfaces_t<abstract_emit_cr_a_t<order_render>>,
          visitor_targets_t<textual_render_path_bits>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;

  void emit(cairo_t *cr) {
    painter_brush_emitter_t::emit(cr);
    cairo_fill_preserve(cr);
  }

  void emit(cairo_t *cr, coordinate_t *a) {
    painter_brush_emitter_t::emit(cr, a);
    cairo_fill_preserve(cr);
  }
};

/**
 @class text_alignment_t
 @brief
 */
class text_alignment_t
    : public storage_emitter_t<
          text_alignment_t, text_alignment_options_t,
          accepted_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};

/**
 * @class text_indent_t
 * @brief
 */
class text_indent_t
    : public storage_emitter_t<
          text_indent_t, double,
          accepted_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};

/**
 * @class text_ellipsize_t
 * @brief
 */
class text_ellipsize_t
    : public storage_emitter_t<
          text_ellipsize_t, text_ellipsize_options_t,
          accepted_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};

/**
 * @class
 * @brief
 */
class text_line_space_t
    : public storage_emitter_t<
          text_line_space_t, double,
          accepted_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};

/**
 * @class text_tab_stops_storage_t
 * @brief holds the tab stop values on the layout
 */
class text_tab_stops_storage_t : virtual public hash_members_t {
public:
  text_tab_stops_storage_t() {}
  text_tab_stops_storage_t(const std::vector<double> &_value) : value(_value) {}
  virtual ~text_tab_stops_storage_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(text_tab_stops_storage_t)));
    for (auto n : value)
      hash_combine(__value, n);
    return __value;
  }

  std::vector<double> value = {};
};

/**
 @class text_tab_stops_t
 @brief
 */
class text_tab_stops_t
    : public class_storage_emitter_t<
          text_tab_stops_t, text_tab_stops_storage_t,
          accepted_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(PangoLayout *layout);
};

/**
 * @typedef text_data_storage_t
 * @brief
 */
typedef std::variant<std::string, std::shared_ptr<std::string>,
                     std::string_view, std::shared_ptr<std::string_view>,
                     std::shared_ptr<std::stringstream>>
    text_data_storage_t;

/**
 * @class
 * @brief
 */
class text_data_t
    : public storage_emitter_t<
          text_data_t, text_data_storage_t,
          accepted_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>> {
public:
  using storage_emitter_t::storage_emitter_t;
  std::size_t hash_code(void) const noexcept;
  void emit(PangoLayout *layout);
};

/**
 * @class
 * @brief
 */
class text_shadow_t
    : public painter_brush_emitter_t<
          text_shadow_t, textual_render_storage_t,
          accepted_interfaces_t<abstract_emit_cr_t<order_render>,
                                abstract_emit_cr_a_t<order_render>>,
          visitor_targets_t<textual_render_normal_bits,
                            textual_render_path_bits>>,
      virtual public pipeline_memory_t,
      virtual public display_visual_t {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;

  ~text_shadow_t() {
    if (layout)
      g_object_unref(layout);
  }

  // interface for visitors
  void emit(cairo_t *cr);
  void emit(cairo_t *cr, coordinate_t *a);

  // private functions
  void pipeline_acquire(cairo_t *cr, coordinate_t *a);
  bool pipeline_has_required_linkages(void);
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_font_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_font_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_color_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_render_path_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_render_normal_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_outline_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_fill_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_alignment_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_indent_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_tab_stops_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_ellipsize_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_line_space_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_tab_stops_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_data_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_shadow_t);
