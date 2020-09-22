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
#pragma once

/**
\author Anthony Matarazzo
\file ux_pipeline_order.hpp
\date 9/7/20
\version 1.0
\brief The file sets the sort order for the priority queue. These
values are set inside the ux_display_unit.hpp file within the
template parameters. One of the order_NNN values is within the
parameter. When objects within the linkages unit memory are
queried for interface match of visitor and it is true,
the push back to the priority queue receives these values.
This makes the items appear in order.

*/

const int order_init = 1;
const int order_render_options = 2;

const int order_before_render = 3;
const int order_render = 4;
const int order_after_render = 5;
const int order_terminate = 6;

template <int T> class pipeline_sort_order_t {
public:
  const int pipeline_stage = T;


  pipeline_sort_order_t() stage(0) {}
  int stage = 0;
};
