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
 * @file image.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 * @details
 */

// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/coordinate.h>

#include <api/image/image.h>

// clang-format on

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
