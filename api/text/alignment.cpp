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
 * @file alignment.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <api/text/alignment.h>
#include <base/unit_object.h>

// clang-format on

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
