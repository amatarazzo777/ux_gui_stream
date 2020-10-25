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
#pragma once

/**
\author Anthony Matarazzo
\file render_base.h
\date 10/21/20
\version 1.0
\brief the file encapsulates the rendering platform base and its interface
technology. The rendering system ues the cairo, glib, librsvg. pango,
pango-cairo, freetype2, harfbuzz, and many more dependencies. Their availability
on multiple platforms with durability in design and thought on performance give
an edge to the amount of code that is require to provide the library. Often, it
is desirable to increase and decrease at the same time. THis can be accomplished
methodically though analysis and break down. Perhaps additional methods can
apply other rendering technology.

*/

#if defined(LINUX_XCB_CAIRO_PANGO_PROCESS_CHAIN)
#include <cairo.h>
#include <cairo-xcb.h>
#include <glib.h>
#include <librsvg/rsvg.h>
#include <pango/pangocairo.h>

#include <base/utility/ptr_type_alias.h>

#include <os/linux_xcb/os.h>

#include <os/linux_xcb/keyboard.h>
#include <os/linux_xcb/mouse.h>
#include <os/linux_xcb/service.h>
#include <os/linux_xcb/window.h>

#endif
