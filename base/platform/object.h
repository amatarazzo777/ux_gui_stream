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
\file object.h
\date 10/21/20
\version 1.0
\brief base include file to create os window manager implementation object.

*/

#include <base/std_base.h>
#include <base/object/layer/hash_interface.h>
#include <base/object/layer/system_error.h>

#include <base/object/display_unit.h>

#include <base/surface/draw_buffer.h>
#include <base/object/layer/hash_interface.h>
#include <base/surface/brush/painter_brush.h>
#include <base/object/layer/system_error.h>
#include <base/object/layer/visitor_interface.h>
#include <base/utility/ptr_type_alias.h>
#include <base/utility/variant_visitor.h>

/// @brief api tie ins
#include <api/enums.h>
#include <api/listeners.h>
