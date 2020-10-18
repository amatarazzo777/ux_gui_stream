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
 * @file ux_image_block_unit.cpp
 * @date 9/25/20
 * @version 1.0
 * @brief
 * @details
 */
#include <ux_compile_options.h>
#include <ux_base.h>
#include <ux_system_error.h>
#include <ux_visitor_interface.h>
#include <ux_hash.h>
#include <ux_enums.h>
#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>
#include <ux_pipeline_memory.h>
#include <ux_display_visual.h>
#include <ux_display_visual.h>
#include <ux_os_window_manager_event_base.h>
#include <ux_os_window_manager_base.h>
#include <ux_display_context.h>
#include <ux_display_unit_base.h>
#include <ux_coordinate.h>
#include <ux_image_block_unit.h>

/**
 * @internal
 * @fn pipeline_acquire
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief create the image rendering pipeline
 * @details The cached rendering function has not been established, create the
 * image rendering pipeline consisting of individualized lambda functions. This
 * functionality can provide image processing capabilities to produce visual
 * effects. Using the emplace back to invoke operations on the image->block_ptr;
 */
void uxdevice::image_block_storage_t::pipeline_acquire() {

  /** compute pipeline that includes rendering commands. The rendering commands
   * are sequenced and appropriate fill, preserve order is maintained.*/
  pipeline_push_visit<fn_emit_cr_t, fn_emit_cr_a_t>();

  /** @brief additional processing can also be inserted here. or may be inserted
   * as a new unit_memory emitter accepting a visitor type.*/

  // add result to buffer
  pipeline_push<order_render>(fn_emit_cr_a_t{
    [&](cairo_t *cr, coordinate_t *a) { image_block.emit(cr, a); }});
}

/**
 * @internal
 * @fn image_block_storage_t::pipeline_has_required_linkages
 * @brief determines if the pipeline has the appropriate linkages or parameters
 * if you will. A validation function.
 */
bool uxdevice::image_block_storage_t::pipeline_has_required_linkages() {
  bool has_requirements = true;

  // validate required parameters
  if (!pipeline_memory_access<coordinate_t>() || description.size() == 0) {
    const char *s = "An image_block_t object must include the following "
                    "attributes. coordinate_t and a description name.";
    error_report(s);
    has_requirements = false;
  }

  return has_requirements;
}

/**
 * @internal
 * @fn image_block_t emit
 * @param display_context_t *context
 * @brief
 */
void uxdevice::image_block_t::emit(display_context_t *context) {
  using namespace std::placeholders;

  if (is_processed)
    return;

  /** this copies the shared pointers from the context to this one, but only
   * named visitor - visitor_image_block_render_t*/
  pipeline_memory_linkages(context, image_block_bits);

  if (!is_valid())
    return;

  // set the ink area.
  coordinate_t &a = *pipeline_memory_access<coordinate_t>();

  /** this function is geared for a thread of an image loading. however, it is
   * not implemented as a thread currently, just named for future.*/
  auto fnthread = [&]() {
    image_block = draw_buffer_t(description, a.w, a.h);

    if (image_block.format == draw_buffer_format_t::none) {
      const char *s = "The image_block_t could not be processed or loaded. ";
      error_report(s);
      error_report(description);

    } else {
      set_ink(a.x, a.y, a.w, a.h);
    }
  };

  fnthread();

  is_processed = true;
  state_hash_code();
}
