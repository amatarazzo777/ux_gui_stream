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
 * @file ux_textual_render.h
 * @date 9/19/20
 * @version 1.0
 * @brief
 */
#include <ux_compile_options.h>
#include <ux_base.h>
#include <ux_error.h>
#include <ux_visitor_interface.h>
#include <ux_hash.h>

#include <ux_enums.h>

#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>

#include <ux_pipeline_memory.h>

#include <ux_display_visual.h>
#include <ux_display_context.h>
#include <ux_display_unit_base.h>

#include <ux_coordinate.h>
#include <ux_text_units.h>

#pragma once

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
class textual_render_storage_t : virtual public hash_members_t,
                                 virtual public system_error_t,
                                 virtual public display_visual_t,
                                 virtual public pipeline_memory_t {
public:
  textual_render_storage_t() {}

  virtual ~textual_render_storage_t() {
    if (layout)
      g_object_unref(layout);
  }

  /// @brief move constructor
  textual_render_storage_t(textual_render_storage_t &&other) noexcept
    : hash_members_t(other), system_error_t(other), display_visual_t(other),
      pipeline_memory_t(other), layout(other.layout), ink_rect(other.ink_rect),
      logical_rect(other.logical_rect) {}

  /// @brief copy constructor
  textual_render_storage_t(const textual_render_storage_t &other)
    : hash_members_t(other), system_error_t(other), display_visual_t(other),
      pipeline_memory_t(other), ink_rect(other.ink_rect),
      logical_rect(other.logical_rect) {
    if (other.layout)
      layout = pango_layout_copy(other.layout);
  }

  /// @brief copy assignment
  textual_render_storage_t &operator=(const textual_render_storage_t &other) {
    hash_members_t::operator=(other);
    system_error_t::operator=(other);
    display_visual_t::operator=(other);
    pipeline_memory_t::operator=(other);

    if (other.layout)
      layout = pango_layout_copy(other.layout);
    ink_rect = other.ink_rect;
    logical_rect = other.logical_rect;
    return *this;
  }

  /// @brief move assignment
  textual_render_storage_t &
  operator=(const textual_render_storage_t &&other) noexcept {
    hash_members_t::operator=(other);
    system_error_t::operator=(other);
    display_visual_t::operator=(other);
    pipeline_memory_t::operator=(other);
    layout = other.layout;
    ink_rect = other.ink_rect;
    logical_rect = other.logical_rect;
    return *this;
  }

  void pipeline_acquire(void);
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
      accepted_interfaces_t<abstract_emit_context_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(display_context_t *context);
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::textual_render_storage_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::textual_render_t)
