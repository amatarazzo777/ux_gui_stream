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
 * @file font.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
// clang-format off

#include <api/text/font.h>
#include <api/text/font_storage.h>
#include <base/unit_object.h>

// clang-format on

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
