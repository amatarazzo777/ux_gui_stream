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
 * @file tab_stops.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief
 */
// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/coordinate.h>
#include <api/text/tab_stops.h>
#include <api/text/tab_stops_storage.h>

// clang-format on

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
