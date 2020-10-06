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

/**
\author Anthony Matarazzo
\file ux_textual_render.cpp
\date Sep 24, 2020
\version 1.0
\brief
*/

#include "ux_device.hpp"

/**

\fn textual_render_storage_t::hash_code(void)

\brief hash code of the textual rendering. used to detect changes
that might occur within all of the dependent rendering parameters,

\details


 */
std::size_t uxdevice::textual_render_storage_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(textual_render_storage_t)),
               pango_layout_get_serial(layout), ink_rect.x, ink_rect.y,
               ink_rect.width, ink_rect.height, matrix.hash_code(),
               pipeline_memory_hash_code());
  return __value;
}

/**
\internal
\fn pipeline

\brief directly calls if exists or decides based upon rendering parameters set
and then calls function. The static

 */
void uxdevice::textual_render_storage_t::pipeline_acquire(cairo_t *cr,
                                                          coordinate_t *a) {
  /**
    @details The cached rendering function has not been established,
    create the text rendering pipeline consisting of individualized lambda
    functions.
  */

  /** @brief
   *
   * these steps are common in the first part of the textual rendering  The
   visitor of the pango, and cairo operate in a sorted fashion to execute
   themselves within a rule based set of order. refinements within the stream
   can occur as successive execution happens but this is not facilitated
   by current code (pipeline_optimize is suffice). Initialization for example,
   and build up of graphics composite layers.

   */


  pipeline_push<order_init>(fn_emit_cr_t{[&](auto cr) {
    if (!layout)
      layout = pango_cairo_create_layout(cr);
  }});

  pipeline_push<order_layout_option>(fn_emit_cr_t{[&](auto cr) {
    // get the serial number of all the current attributes.
    // if any of the emitter functions change the layout
    // the serial number changes. the push visit below
    // accomplishes this
    layout_serial = pango_layout_get_serial(layout);
  }});

  /**
   * @brief
   *   emit all items that effect pango objects as labeled with attribute
   * settings. Notice that the appropriate ones are visited here based upon
   * the visitor name, and its parameters. this visits all items that effect
   * the pango layout but does not function as a drawing operator.
   *
   */
  pipeline_push_visit<fn_emit_layout_t>();

  /**
   * @brief check the pango serial. set the ink area according to the pixel
   * metrics and update the layout.
   */
  pipeline_push<order_render_option>(fn_emit_cr_t{[&](auto cr) {
    // any changes
    if (layout_serial != pango_layout_get_serial(layout)) {
      auto coordinate = pipeline_memory_access<coordinate_t>();
      pango_layout_get_pixel_extents(layout, &ink_rect, &logical_rect);
      int tw = std::min((double)logical_rect.width, coordinate->w);
      int th = std::min((double)logical_rect.height, coordinate->h);
      ink_rectangle = {(int)coordinate->x, (int)coordinate->y, tw, th};
      ink_rectangle_double = {(double)ink_rectangle.x, (double)ink_rectangle.y,
                              (double)ink_rectangle.width,
                              (double)ink_rectangle.height};

      has_ink_extents = true;
      pango_cairo_update_layout(cr, layout);
    }
  }});

  // compute pipeline that includes rendering commands. The rendering
  // commands are sequenced and appropriate fill, preserve order is
  // maintained.
  pipeline_push_visit<fn_emit_cr_a_t>();

  return;
}

/**
\internal
\fn pipeline_has_required_linkages

\brief

 */
bool uxdevice::textual_render_storage_t::pipeline_has_required_linkages(void) {
  if (!((pipeline_memory_access<text_color_t>() ||
         pipeline_memory_access<text_outline_t>() ||
         pipeline_memory_access<text_fill_t>()) &&
        pipeline_memory_access<coordinate_t>() &&
        pipeline_memory_access<text_data_t>() &&
        pipeline_memory_access<text_font_t>())) {
    const char *s = "A textual_render_t object must include the following "
                    "attributes: A text_color_t, text_outline_t or "
                    " text_fill_t. As well, a coordinate_t, text and "
                    "text_font_t object.";
    error_report(s);
    return false;
  }
  return true;
}

/**
\internal
\fn emit
\param display_context_t &context

\brief creates linkages to the drawing functions and rendering parameters

\details


 */
void uxdevice::textual_render_t::emit(display_context_t *context) {
  using namespace std::placeholders;

  if (is_processed)
    return;

  // this copies the shared pointers from the context
  // to this one, but only named visitor - visitor_textual_render_t
  pipeline_memory_linkages(context);

  // this adds a parameter for the specific object
  pipeline_memory_store<PangoLayout *>(layout);


  is_processed = true;
}



