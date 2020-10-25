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
 * @file ellipsize.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
// clang-format off

#include <api/text/ellipsize.h>
#include <base/unit_object.h>
// clang-format on

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
