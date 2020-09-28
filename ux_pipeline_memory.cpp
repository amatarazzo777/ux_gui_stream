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
\file uxunitmemory.cpp
\date 9/7/20
\version 1.0
\brief
*/
#include "ux_device.hpp"

/**
  \fn unit_memory_linkages

*/
template <typename ATTR>
void uxdevice::pipeline_memory_t<ATTR>::unit_memory_linkages(
    display_context_t &other) {
  for (auto n : other.storage)
    if (n.first == std::type_index(typeid(ATTR)))
      storage[n.first] = n.second;
}

void pipeline_execute(display_context_t *context) {

  auto overload_function_visitor = overload_visitors_t{
      [&](fn_emit_cr_t &fn) { fn(context->cr); },
      [&](fn_emit_context_t &fn) { fn(context); },

      [&](fn_emit_cr_a_t fn) {
        fn(context->cr, pipeline_memory_access<coordinate_t>().get());
      },

      [&](fn_emit_cr_a_absolute_t fn) {
        fn(context->cr, pipeline_memory_access<coordinate_t>().get());
      },

      [&](fn_emit_cr_a_relative_t fn) {
        fn(context->cr, pipeline_memory_access<coordinate_t>().get());
      },

      [&](fn_emit_layout_t fn) {
        fn(pipeline_memory_access<PangoLayout *>().get());
      },

      [&](fn_emit_layout_a_t fn) {
        fn(pipeline_memory_access<PangoLayout *>().get(),
           pipeline_memory_access<coordinate_t>().get());
      },

      [&](fn_emit_cr_layout_t fn) {
        fn(context->cr, pipeline_memory_access<PangoLayout *>().get());
      }};
  // arrange pipeline if necessary
  if (!bfinalized)
    pipeline_finalize();

  // if the pipeline is in a ready state.
  // providing broad functionality allow for expansion if necessary.
  if (pipeline_ready()) {
    // pipeline has already been sorted.
    for (auto o : pipeline_io) {
      auto overloaded_function = std::get<fn_emit_overload_t>(o);
      std::visit(overload_function_visitor, overloaded_function);
    }
  }
}
