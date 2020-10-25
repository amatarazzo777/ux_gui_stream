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
 @author Anthony Matarazzo
 @file ux_text_units.h
 @date 9/7/20
 @version 1.0
 @brief The file fulfills the necessary inclusions for an object such as a unit
 to be easily declared in h an cpp file.


 */

#pragma once
// clang-format off
#include <base/std_base.h>

#include <api/options.h>

#include <base/object/layer/system_error.h>
#include <base/object/layer/hash_interface.h>

#include <base/platform/keyboard.h>
#include <base/platform/mouse.h>
#include <base/platform/service.h>
#include <base/platform/window_interface.h>
#include <base/platform/message_queue.h>
#include <base/platform/window.h>

/// @brief os dependencies resolved in namespace type here.
/// based upon compiler flag detection __linux__, WIN64, etc.
#include <base/render_base.h>

/// @brief utility functions. The variant_visitor, ptr_type_alias work together
/// to alias pointer types from the xcb dispatch. Other implementations may not
/// require them

#include <base/utility/variant_visitor.h>

#include <base/utility/cairo_function.h>

#include <api/enums.h>
#include <api/listeners.h>
#include <api/matrix.h>
#include <api/options.h>

#include <base/object/display_unit.h>
#include <base/object/display_visual.h>
#include <base/object/layer/pipeline_memory.h>

#include <base/object/layer/visitor_interface.h>

#include <base/surface/display_context.h>
#include <base/surface/draw_buffer.h>
#include <base/surface/brush/painter_brush.h>

/// @brief object factories for declaring it in a compact form within the unit
/// .h file. These are template based except for fn_storage_emitter which can be
/// created by a lambda function.
#include <base/factory/class_storage_emitter.h>
#include <base/factory/fn_storage_emitter.h>
#include <base/factory/marker_emitter.h>
#include <base/factory/painter_brush_emitter.h>
#include <base/factory/storage_emitter.h>

// clang-format on
