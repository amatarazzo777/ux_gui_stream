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
\file ux_image_block_unit.hpp
\date 9/25/20
\version 1.0

\brief
\details


*/

#include "ux_device.hpp"

/**
\internal
\fn pipeline

\brief create the image rendering pipeline

 */
void uxdevice::image_block_storage_t::pipeline_acquire() {

  /**
    \details The cached rendering function has not been established,
    create the image rendering pipeline consisting of individualized lambda
    functions. This functionality can provide image processing capabilities
    to produce visual effects. Using the emplace back to invoke operations
    on the image->block_ptr;
  */

  /// @brief these steps are common in the first part of the image rendering
  pipeline_push<order_render_option>(
      fn_emit_cr_t{[](auto cr) { matrix.emit(cr); }});

  // compute pipeline that includes rendering commands. The rendering commands
  // are sequenced and appropriate fill, preserve order is maintained.
  pipeline_push_visit<fn_emit_cr_t, fn_emit_cr_a_t>();

  /// @brief additional processing can also be inserted here. or
  /// may be inserted as a new unit_memory emitter accepting a visitor type.

  // add result to buffer
  pipeline_push<order_render>(
      fn_emit_cr_a_t{[](auto cr, auto a) { image_block.emit(cr, a); }});
}

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
\internal
\brief
*/
void uxdevice::image_block_t::emit(display_context_t *context) {
  using namespace std::placeholders;

  if (is_processed)
    return;

  if (!is_valid())
    return;

  // set the ink area.
  coordinate_t &a = *access_pipeline_memory<coordinate_t>();

  // this function is geared for a thread of an image loading.
  // however, it is not implemented as a thread currently, just named for
  // future.
  auto fnthread = [&]() {
    image_block = draw_buffer_t(description, coordinate->w, coordinate->h);

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
