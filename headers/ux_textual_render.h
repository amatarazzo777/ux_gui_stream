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
 * @author Anthony Matarazzo
 * @file ux_textual_render.h
 * @date 9/19/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {

/**
 * @class textual_render_storage_t
 * @brief class used to store parameters and options for a textual render. The
 * object is created as the side effect of inserting text, char *, std string or
 * a std::shared_ptr<std::string>. The class inherits from display_visual_t
 * which notes the ink rectangle of the object. The display_visual_t class also
 * exposes an intersection method for the render system to decide which function
 * to invoke, a standard draw or a clipped draw.
 */
class textual_render_storage_t
    : virtual public display_visual_t,
      virtual public pipeline_memory_t<visitor_textual_render_t>,
      virtual public hash_members_t,
      virtual public system_error_t {
public:
  textual_render_storage_t() {}

  virtual ~textual_render_storage_t() {
    if (layout)
      g_object_unref(layout);
  }

  /// @brief move constructor
  textual_render_storage_t(textual_render_storage_t &&other) noexcept
      : layout(other.layout), ink_rect(other.ink_rect),
        logical_rect(other.logical_rect) {}

  /// @brief copy constructor
  textual_render_storage_t(const textual_render_storage_t &other)
      : ink_rect(other.ink_rect), logical_rect(other.logical_rect) {
    if (other.layout)
      layout = pango_layout_copy(other.layout);
  }

  /// @brief copy assignment
  textual_render_storage_t &operator=(const textual_render_storage_t &other) {
    if (other.layout)
      layout = pango_layout_copy(other.layout);
    ink_rect = other.ink_rect;
    logical_rect = other.logical_rect;
    return *this;
  }

  /// @brief move assignment
  textual_render_storage_t &
  operator=(const textual_render_storage_t &&other) noexcept {
    layout = other.layout;
    ink_rect = other.ink_rect;
    logical_rect = other.logical_rect;
    return *this;
  }

  void pipeline_acquire(cairo_t *cr, coordinate_t *a);
  bool pipeline_has_required_linkages(void);
  std::size_t hash_code(void) const noexcept;

  PangoLayout *layout = nullptr;
  guint layout_serial = {};
  PangoRectangle ink_rect = PangoRectangle();
  PangoRectangle logical_rect = PangoRectangle();
};

/**
 * @class textual_render_t
 * @brief
 */
class textual_render_t
    : public class_storage_emitter_t<
          textual_render_t, textual_render_storage_t,
          visitor_interfaces_t<abstract_emit_context_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(display_context_t *context);
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::textual_render_storage_t);
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::textual_render_t);