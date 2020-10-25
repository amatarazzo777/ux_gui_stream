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
 * @file textual_render.cpp
 * @date Sep 24, 2020
 * @version 1.0
 * @brief
 */
// clang-format off

#include <base/unit_object.h>
#include "textual_render_storage.h"
#include "textual_render.h"

// clang-format on

/**
 * @internal
 * @fn textual_render_t::emit
 * @param display_context_t *context
 * @brief creates linkages to the drawing functions and rendering parameters
 */
void uxdevice::textual_render_t::emit(display_context_t *context) {

  if (is_processed)
    return;

  // this copies the shared pointers from the context
  // to this one, but only named visitor - visitor_textual_render_t

  if (context->pipeline_memory_access<text_render_normal_t>())
    pipeline_memory_linkages(context, textual_render_normal_bits);
  else if (context->pipeline_memory_access<text_render_path_t>())
    pipeline_memory_linkages(context, textual_render_path_bits);

  // this adds a parameter for the specific object
  pipeline_memory_store<PangoLayout *>(layout);

  is_processed = true;
}
