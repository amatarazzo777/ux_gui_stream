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
 * @file ux_surface_area_units.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
#include <ux_base.h>
#include <ux_system_error.h>
#include <ux_hash.h>
#include <ux_enums.h>
#include <ux_visitor_interface.h>
#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>
#include <ux_pipeline_memory.h>

#include <ux_display_visual.h>
#include <ux_display_context.h>
#include <ux_display_unit_base.h>
#include <ux_event_listeners.h>
#include <ux_os_window_manager_event_base.h>
#include <ux_os_window_manager_base.h>

#include <ux_coordinate.h>
#include <ux_surface_area_units.h>

/**
 * @internal
 * @fn surface_area_title_t
 * @param display_context_t &context
 * @brief Sets the window title.
 */
void uxdevice::surface_area_title_t::emit(display_context_t *context) {
  // set window title
#if 0
  xcb_change_property(context->window_manager->connection,
                      XCB_PROP_MODE_REPLACE, context->window, XCB_ATOM_WM_NAME,
                      XCB_ATOM_STRING, 8, value.size(), value.data());
#endif
}

/**
 * @internal
 * @fn surface_area_brush_t
 * @param display_context_t &context
 * @brief Sets the background brysh.
 */
void uxdevice::surface_area_brush_t::emit(display_context_t *context) {
  context->surface_brush(*this);
}
