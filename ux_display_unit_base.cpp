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
 * @file ux_os_xcb_linux.h
 * @date Oct 12, 2020
 * @version 1.0
 * @details The file implements the os interface which provides the
 * communication to the os for basic visualization services such as window
 * manager functions. The message queue processor is defined and implemented
 * here.
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
#include <ux_display_visual.h>
#include <ux_display_unit_base.h>
#include <ux_pipeline_memory.h>
#include <ux_display_context.h>
#include <ux_event_listeners.h>
#include <ux_os_window_manager_event_base.h>
#include <ux_os_window_manager_base.h>
#include <ux_coordinate.h>

#if 0
void uxdevice::painter_brush_emitter_t::emit(
  uxdevice::display_context_t *context) {
  context->window_manager->draw_fn([&](auto cr) { painter_brush_t::emit(cr); });
}

/**
 * @fn void emit(cairo_t*)
 * @brief
 *
 * @param cr
 */
void uxdevice::painter_brush_emitter_t::emit(cairo_t *cr) {
  painter_brush_t::emit(cr);
}

/**
 * @fn void emit(cairo_t*, coordinate_t*)
 * @brief
 *
 * @param cr
 * @param a
 */
void uxdevice::painter_brush_emitter_t::emit(cairo_t *cr, coordinate_t *a) {
  painter_brush_t::emit(cr, a);
}
#endif
