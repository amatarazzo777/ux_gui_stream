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
 * @author Anthony Matarazzo
 * @file ux_system_base.h
 * @date 9/7/20
 * @version 1.0
 * @brief include file references all objects within the system for the
 * instantiation of the compiled interface.
 */

// clang-format off

/// @brief coordinate
#include <api/coordinate/coordinate_storage.h>
#include <api/coordinate/coordinate.h>
#include <api/coordinate/relative.h>
#include <api/coordinate/absolute.h>

/// @brief image
#include <api/image/image_storage.h>
#include <api/image/image.h>

/// @brief operators
#include <api/operator/fill.h>
#include <api/operator/mask.h>
#include <api/operator/paint.h>
#include <api/operator/stroke_fill_storage.h>
#include <api/operator/stroke_fill.h>
#include <api/operator/stroke.h>
#include <api/operator/fill.h>

/// @brief drawing options
#include <api/option/antialias.h>
#include <api/option/graphic_operator.h>
#include <api/option/line_cap.h>
#include <api/option/line_dash_storage.h>
#include <api/option/line_dashes.h>
#include <api/option/line_join.h>
#include <api/option/line_width.h>
#include <api/option/miter_limit.h>
#include <api/option/tollerance.h>

/// @brief path drawing
#include <api/path/arc_storage.h>
#include <api/path/arc.h>
#include <api/path/close_path.h>
#include <api/path/curve_storage.h>
#include <api/path/curve.h>
#include <api/path/hline.h>
#include <api/path/line_storage.h>
#include <api/path/line.h>
#include <api/path/negative_arc_storage.h>
#include <api/path/negative_arc.h>
#include <api/path/vline.h>

/// @brief shape drawing
#include <api/shape/rectangle_storage.h>
#include <api/shape/rectangle.h>

/// @brief control the surface window
#include <api/surface/brush.h>
#include <api/surface/title.h>

/// @brief text units
#include <api/text/alignment.h>
#include <api/text/color.h>
#include <api/text/data.h>
#include <api/text/ellipsize.h>
#include <api/text/fill.h>
#include <api/text/font_storage.h>
#include <api/text/font.h>
#include <api/text/indent.h>
#include <api/text/line_space.h>
#include <api/text/normal.h>
#include <api/text/outline.h>
#include <api/text/path.h>
#include <api/text/shadow.h>
#include <api/text/tab_stops_storage.h>
#include <api/text/tab_stops.h>

// clang-format on
