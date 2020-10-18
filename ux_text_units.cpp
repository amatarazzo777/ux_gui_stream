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
 * @file ux_text_units.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
#include <ux_compile_options.h>
#include <ux_base.h>
#include <ux_system_error.h>
#include <ux_variant_visitor.h>
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

#include <ux_textual_render.h>
#include <ux_text_units.h>

/**
 * @internal
 * @fn text_render_normal_t::emit
 * @param display_context_t &context
 * @brief uses cached glyph bitmap rendering. fastest method of display using
 * bitmap font cache data. The emit function removes the text_render_path_t from
 * the display memory as they are exclusive
 * @details
 */
void uxdevice::text_render_normal_t::emit(display_context_t *context) {
  context->pipeline_memory_reset<text_render_path_t>();
}

/**
 * @internal
 * @fn text_render_normal_t::emit
 * @param cairo_t *cr
 * @param PangoLayout *layout
 * @brief invokes the pango api
 * @details

 */
void uxdevice::text_render_normal_t::emit(cairo_t *cr, PangoLayout *layout) {
  pango_cairo_show_layout(cr, layout);
}

/**
 * @internal
 * @fn text_render_path_t::emit
 * @param display_context_t &context
 * @brief outputs text using vector drawing operations. This enables the custom
 * outline and fill of text. Slower but more versatile. Text can bend to follow
 * a path if logic is added.
 */
void uxdevice::text_render_path_t::emit(display_context_t *context) {
  context->pipeline_memory_reset<text_render_normal_t>();
}

/**
 * @internal
 * @fn text_render_path_t::emit
 * @param cairo_t *cr
 * @param PangoLayout *layout
 * @brief invokes the pango api to render font text as a path from the layout
 */
void uxdevice::text_render_path_t::emit(cairo_t *cr, PangoLayout *layout) {
  pango_cairo_layout_path(cr, layout);
}

/**
 * @internal
 * @fn text_alignment_t::emit
 * @param PangoLayout *layout
 * @brief sets the alignment mode when wrapping, left, right, center, justified.
 */
void uxdevice::text_alignment_t::emit(PangoLayout *layout) {
  PangoAlignment correlated_value = static_cast<PangoAlignment>(value);
  if (value == text_alignment_options_t::justified &&
      !pango_layout_get_justify(layout)) {
    pango_layout_set_justify(layout, true);
  } else if (pango_layout_get_alignment(layout) != correlated_value) {
    pango_layout_set_justify(layout, false);
    pango_layout_set_alignment(layout, correlated_value);
  }
}

/**
 * @internal
 * @fn text_indent_t::emit
 * @param PangoLayout *layout
 * @brief indentation of first line paragraph.
 */
void uxdevice::text_indent_t::emit(PangoLayout *layout) {
  int pangoUnits = value * PANGO_SCALE;
  pango_layout_set_indent(layout, pangoUnits);
}

/**
 * @internal
 * @fn text_ellipsize_t::emit
 * @param PangoLayout *layout
 * @brief when text is longer than the defined box, "..." is shown. can be at
 * beginning, middle, or end.
 */
void uxdevice::text_ellipsize_t::emit(PangoLayout *layout) {
  pango_layout_set_ellipsize(layout, static_cast<PangoEllipsizeMode>(value));
}

/**
 * @internal
 * @fn text_line_space_t::emit
 * @param PangoLayout *layou
 * @brief space between the lines of text.
 */
void uxdevice::text_line_space_t::emit(PangoLayout *layout) {
  pango_layout_set_line_spacing(layout, static_cast<float>(value));
}

/**
 * @internal
 * @fn text_tab_stops_t::emit
 * @param PangoLayout *layout
 * @brief when tabs occur, this defines the tab stops.
 */
void uxdevice::text_tab_stops_t::emit(PangoLayout *layout) {
  PangoTabArray *tabs = pango_tab_array_new(value.size(), true);

  int idx = 0;
  for (auto &tabdef : value) {
    int loc = static_cast<int>(tabdef);
    pango_tab_array_set_tab(tabs, idx, PANGO_TAB_LEFT, loc);
  }
  pango_layout_set_tabs(layout, tabs);
  pango_tab_array_free(tabs);
}

/**
 * @internal
 * @fn text_font_t::emit
 * @param PangoLayout *layout
 */
void uxdevice::text_font_t::emit(PangoLayout *layout) {
  if (!font_ptr) {
    font_ptr = pango_font_description_from_string(description.data());
    if (!font_ptr) {
      std::string s = "Font could not be loaded from description. ( ";
      s += description + ")";
      error_report(s);
    }
  }

  const PangoFontDescription *internal_description =
    pango_layout_get_font_description(layout);
  if (!internal_description ||
      !pango_font_description_equal(internal_description, font_ptr))
    if (font_ptr)
      pango_layout_set_font_description(layout, font_ptr);
}

/**
 * @internal
 * @fn text_outline_t::emit
 * @param cairo_t *cr
 * @brief   since class implements the painter_brush_emitter_t , specializations
 * may call the base constructor,
 *
 */
void uxdevice::text_outline_t::emit(cairo_t *cr) {
  double dwidth = cairo_get_line_width(cr);
  cairo_set_line_width(cr, _width);
  painter_brush_emitter_t::emit(cr);
  cairo_stroke(cr);
  cairo_set_line_width(cr, dwidth);
}

/**
 * @internal
 * @fn text_outline_t::emit
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief   since class implements the painter_brush_emitter_t , specializations
 * may call the base constructor,
 *
 */
void uxdevice::text_outline_t::emit(cairo_t *cr, coordinate_t *a) {
  double dwidth = cairo_get_line_width(cr);
  cairo_set_line_width(cr, _width);
  painter_brush_emitter_t::emit(cr, a);
  cairo_stroke(cr);
  cairo_set_line_width(cr, dwidth);
}

/**
 * @internal
 * @fn text_shadow_t::emit
 * @param cairo_t *cr
 * @brief   s shows the text shadow
 *
 */
void uxdevice::text_shadow_t::emit(cairo_t *cr) { internal_buffer.emit(cr); }

/**
 * @internal
 * @fn text_shadow_t::emit
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief  shows the text shadow
 *
 */
void uxdevice::text_shadow_t::emit(cairo_t *cr, coordinate_t *a) {
  internal_buffer.emit(cr, a);
}

/**
 * @internal
 * @fn text_shadow_t::pipeline_acquire
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief
 *
 */
void uxdevice::text_shadow_t::pipeline_acquire() {

  pipeline_push<order_render>(fn_emit_cr_a_t{
    [&](cairo_t *cr, coordinate_t *a) {
      // draw the text in shadow color
      if (internal_buffer) {
        internal_buffer.emit(cr, a);
        return;
      }

      internal_buffer =
        draw_buffer_t(a->w + x + radius * 2, a->h + y + radius * 2);

      pipeline_disable_visit<text_shadow_t>();
      pipeline_push<order_render>(
        fn_emit_cr_a_t{[&](cairo_t *cr, coordinate_t *a) {
          internal_buffer.flush();
          internal_buffer.blur_image(radius);

          internal_buffer.emit(cr, a);
        }});
    }
    // pipeline_execute();
  });
}

/**
 * @internal
 * @fn text_shadow_t::pipeline_has_required_linkages
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief determines if the textual drawing parameters for shadow are all stored
 * within the pipeline memory.
 * @return bool
 */
bool uxdevice::text_shadow_t::pipeline_has_required_linkages(void) {
  return textual_render_storage_t::pipeline_has_required_linkages();
}

/**
 * @internal
 * @fn text_data_t::hash_code
 * @brief
 * @return std::size_t - the hashed value
 */
std::size_t uxdevice::text_data_t::hash_code(void) const noexcept {
  std::size_t __value = std::type_index(typeid(text_data_t)).hash_code();

  auto text_data_visitor = overload_visitors_t{
    [&](std::string s) { hash_combine(__value, s); },
    [&](std::string_view s) { hash_combine(__value, s); },
    [&](std::shared_ptr<std::string> ps) { hash_combine(__value, *ps); },
    [&](std::shared_ptr<std::string_view> ps) { hash_combine(__value, *ps); },
    [&](std::shared_ptr<std::stringstream> ps) {
      std::string stmp = ps->str();
      hash_combine(__value, stmp);
    }};

  std::visit(text_data_visitor, value);

  return __value;
}

/**
 * @internal
 * @fn text_data_t::emit
 * @param PangoLayout *layout
 * @brief sets that text of the layout to the one stored withi nthe text_data_t
 * object. The text_data_t object contains a text_data_storage_t which is a
 * std::variant holding the information within a string, string_view or
 */
void uxdevice::text_data_t::emit(PangoLayout *layout) {
  std::string_view sinternal = std::string_view(pango_layout_get_text(layout));
  auto text_data_visitor =
    overload_visitors_t{[&](std::string &s) {
                          if (s.compare(sinternal) != 0)
                            pango_layout_set_text(layout, s.data(), -1);
                        },

                        [&](std::string_view &s) {
                          if (s.compare(sinternal) != 0)
                            pango_layout_set_text(layout, s.data(), -1);
                        },

                        [&](std::shared_ptr<std::string> ps) {
                          if (ps->compare(sinternal) != 0)
                            pango_layout_set_text(layout, ps->data(), -1);
                        },

                        [&](std::shared_ptr<std::string_view> ps) {
                          if (ps->compare(sinternal) != 0)
                            pango_layout_set_text(layout, ps->data(), -1);
                        },

                        [&](std::shared_ptr<std::stringstream> ps) {
                          std::string stmp = ps->str();
                          if (stmp.compare(sinternal) != 0)
                            pango_layout_set_text(layout, stmp.data(), -1);
                        }};

  std::visit(text_data_visitor, value);
}
