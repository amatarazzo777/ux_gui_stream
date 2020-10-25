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
 * @file relative.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/relative.h>
#include <api/coordinate/absolute.h>
// clang-format on

/**
 * @internal
 * @fn emit
 * @param display_context_t *context
 * @brief
 */
void uxdevice::relative_coordinate_t::emit(display_context_t *context) {
  context->pipeline_memory_reset<absolute_coordinate_t>();
}
