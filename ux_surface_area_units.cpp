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
\file uxdisplayunits.hpp
\date 9/7/20
\version 1.0

\brief emit implementations for objects.

\details

The modules extends the uxdevice namespace. The objects
provided are the base objects for which the caller may instantiate to
draw either text or line.

While most of these objects are parameters to drawing functions, the
implementation with in this file provides the functional logic. All
objects derive from the display_unit_t class which contains the virtual
abstracts named for the object created from the template class.
Objects that provide drawing operations can inherit from
the display_visual_t base class which enables visibility query.


*/

#include "ux_device.hpp"

/**

\fn surface_area_title_t
\param display_context_t &context

\brief Sets the window title.

 */
void uxdevice::surface_area_title_t::emit(display_context_t *context) {
  // set window title
  xcb_change_property(context.connection, XCB_PROP_MODE_REPLACE, context.window,
                      XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, value.size(),
                      value.data());
}

/**

\fn surface_area_brush_t
\param display_context_t &context

\brief Sets the background brysh.

 */
void uxdevice::surface_area_brush_t::emit(display_context_t *context) {
  context->surface_brush(*this);
}