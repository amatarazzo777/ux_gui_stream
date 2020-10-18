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
\file ux_compile_options.hpp
\date 9/19/20
\version 1.0
\brief Library Build Options

*/

/**
\addtogroup Library Build Options
\brief Library Options
\details These options provide the selection to configure selection
options when compiling the text_color_t.
@{
*/

/**
\internal
\def DEFAULT_WINDOW_TITLE
\brief when a window title is not provided, this is used.

*/
#define DEFAULT_WINDOW_TITLE                                                   \
  std::string(__FILE__) + std::string("  ") + std::string(__DATE__)

/**
\internal
\def SYSTEM_DEFAULTS
\brief sets the defaults for each window created.

*/
#define SYSTEM_DEFAULTS                                                        \
  in(absolute_coordinate_t(), text_render_normal_t{},                          \
     text_font_t{"Arial 20px"}, text_color_t{"black"},                         \
     surface_area_brush_t{"white"}, text_indent_t{100.0},                      \
     text_alignment_t{text_alignment_options_t::left},                         \
     text_ellipsize_t{text_ellipsize_options_t::off}, text_line_space_t{1.0},  \
     text_tab_stops_t{                                                         \
       std::vector<double>{250, 250, 250, 250, 250, 250, 250, 250}},           \
     surface_area_title_t{DEFAULT_WINDOW_TITLE});

/**
\def USE_STACKBLUR
\brief The stack blue algorithm of shadow creation is used. Use either
 USE_STACKBLUR or USE_SVGREN. Both cannot be used at the same time.
*/
//#define USE_STACKBLUR

/**
\def USE_SVGREN
\brief The svgren svg c++ algorithm of image blurring is used. A convolution
matrix is applied. Much slower but may look a bit better. Use either
 USE_STACKBLUR or USE_SVGREN. Both cannot be used at the same time.
*/

#define USE_SVGREN

// catch for dual defines. must comment out one of the options.
#ifdef USE_STACKBLUR
#ifdef USE_SVGREN
#error "Select either USE_STACKBLUR or USE_SVGREN but not both."
#endif // USE_SVGREN
#endif

/**
\def USE_DEBUG_CONSOLE
*/
#define USE_DEBUG_CONSOLE
#define CONSOLE
/** @} */
