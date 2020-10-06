
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
 \file uxdisplayunits.hpp
 \date 9/7/20
 \version 1.0
 \brief

The file encapsulates the objects that may be created, controlled, and rendered
by the system. The system provides for std::shared pointers to these object to
be shared within the rendering cycle. Each object has a hash function which is
queried to detect changes within the underlying data. Only on screen related
objects are included within the set. The intersection test is included as part
of a base hierarchy of classes for objects that are declared as such. The class
objects are separated into two distinct entity names. One that is the API name
and a data storage class.


painter_brush_emitter_t
marker_emitter_t
storage_emitter_t
class_storage_emitter_t



 */

/********************************************************************************

 API objects

 *********************************************************************************

 The following definitions are the exact name that appears within the API. These
 class templates provide factories that create objects compatible with the
 system. These objects might have invoke and emit methods depending upon the
 emit abstract interfaces named for inheritance. The base services for these
 objects to be functional for the display are provided. These objects have all
 of the move and copy operators implemented for ease and efficiency of use.
 There are several types of objects that may be created and inserted by using
 the template parameters. Please refer to the class template for a more detailed
 explanation. The following templates are provided for use:


 marker_emitter_t
 painter_brush_emitter_t
 storage_emitter_t
 class_storage_emitter_t



 *********************************************************************************/

/**
 \internal
 \class text_font_data_storage
 \brief storage for fonts.

 \details


 */
namespace uxdevice {
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
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_font_storage_t);


/**
 \class text_font_t
 \brief
 */
namespace uxdevice {
using text_font_t = class text_font_t
    : public class_storage_emitter_t<
          text_font_t, text_font_storage_t,
          visitor_interfaces_t<abstract_emit_layout_t<order_render_option>>,
          visitor_textual_render_t> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;
  void emit(PangoLayout *layout);

};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_font_t);

/**
 \class text_render_normal_t
 \brief

 The inclusion abstract_emit_context_t is used to
 allow the exclusion of the counterpart attribute class
 text_render_path_t within the emit function.

 The abstract_emit_cr_a_t provides for the inclusion
 of delegation during visitation by attached objects.

 Objects may attach themselves by different interfaces named by the
 visitor pattern.

 The pipeline sort order informs the queue order.

 */

namespace uxdevice {
using text_render_normal_t = class text_render_normal_t
    : public marker_emitter_t<
          text_render_normal_t,
          visitor_interfaces_t<abstract_emit_context_t<order_init>,
                               abstract_emit_cr_layout_t<order_render>>,
          visitor_textual_render_t> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
  void emit(cairo_t *cr, PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_render_normal_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using text_render_path_t = class text_render_path_t
    : public marker_emitter_t<
          text_render_path_t,
          visitor_interfaces_t<abstract_emit_context_t<order_init>,
                               abstract_emit_cr_layout_t<order_render>>,
          visitor_textual_render_t> {
public:
  using marker_emitter_t::marker_emitter_t;

  void emit(display_context_t *context);
  void emit(cairo_t *cr, PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_render_path_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using text_color_t = class text_color_t
    : public painter_brush_emitter_t<
          text_color_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render_option>,
                               abstract_emit_cr_a_t<order_render_option>>,
          visitor_textual_render_t> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;
  void emit(cairo_t *cr) { painter_brush_emitter_t::emit(cr); }
  void emit(cairo_t *cr, coordinate_t *a) {
    painter_brush_emitter_t::emit(cr, a);
  }
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_color_t);

/**
 \class text_outline_t
 \brief


 */
namespace uxdevice {
using text_outline_t = class text_outline_t
    : public painter_brush_emitter_t<
          text_outline_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render>,
                               abstract_emit_cr_a_t<order_render>>,
          visitor_textual_render_t> {
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
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_outline_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using text_fill_t = class text_fill_t
    : public painter_brush_emitter_t<
          text_fill_t, visitor_interfaces_t<abstract_emit_cr_a_t<order_render>>,
          visitor_textual_render_t> {
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
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_fill_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using text_alignment_t = class text_alignment_t
    : public storage_emitter_t<
          text_alignment_t, text_alignment_options_t,
          visitor_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_alignment_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using text_indent_t = class text_indent_t
    : public storage_emitter_t<
          text_indent_t, double,
          visitor_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_indent_t);

/**

 \class
 text_tab_stops_storage_t
 \brief sets the tab stops on
 the layout
 */

namespace uxdevice {
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
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_tab_stops_storage_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using text_ellipsize_t = class text_ellipsize_t
    : public storage_emitter_t<
          text_ellipsize_t, text_ellipsize_options_t,
          visitor_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_ellipsize_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using text_line_space_t = class text_line_space_t
    : public storage_emitter_t<
          text_line_space_t, double,
          visitor_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_line_space_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using text_tab_stops_t = class text_tab_stops_t
    : public class_storage_emitter_t<
          text_tab_stops_t, text_tab_stops_storage_t,
          visitor_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_textual_render_t> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_tab_stops_t);

/**
 \class
 \brief
 */
namespace uxdevice {
typedef std::variant<std::string, std::shared_ptr<std::string>,
                     std::string_view, std::shared_ptr<std::string_view>,
                     std::shared_ptr<std::stringstream>>
    text_data_storage_t;

using text_data_t = class text_data_t
    : public storage_emitter_t<
          text_data_t, text_data_storage_t,
          visitor_interfaces_t<abstract_emit_layout_t<order_layout_option>>,
          visitor_textual_render_t> {
public:
  using storage_emitter_t::storage_emitter_t;
  std::size_t hash_code(void) const noexcept;
  void emit(PangoLayout *layout);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_data_t);

/**
 \class
 \brief

 */
namespace uxdevice {
using text_shadow_t = class text_shadow_t
    : public painter_brush_emitter_t<
          text_shadow_t, textual_render_storage_t,
          visitor_interfaces_t<abstract_emit_cr_t<order_render>,
                               abstract_emit_cr_a_t<order_render>>,
          visitor_textual_render_t>,
      virtual public pipeline_memory_t<visitor_textual_render_t>,
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
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_shadow_t);
