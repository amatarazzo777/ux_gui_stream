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
void uxdevice::pipeline_memory_t<ATTR>::pipeline_memory_linkages(
    display_context_t &other) {
  for (auto n : other.storage)
    if (n.first == std::type_index(typeid(ATTR)))
      storage[n.first] = n.second;
}

/**
  \fn pipeline_execute
  \param display_context_t *context
  \brief function visits the pipeline sequentially
      executing the pipeline lambda using the context parameter.
      Parameters for visitors use the pipeline_memory_access
      routine and pass the type requested.

*/
// template <typename ATTR>
// void uxdevice::pipeline_memory_t<ATTR>::pipeline_execute(
void uxdevice::pipeline_acquisition_t::pipeline_execute(
    display_context_t *context) {

  // all types have to be declared here in the visitor
  // The compile errors are very difficult to relate
  // to this. Each of the std::functions have to have a unique
  // prototype signature. This is true even if the
  // class has a different name. Notice that the
  // abstract class abstract_emit_cr_relative_t and
  // abstract_emit_cr_absolute_t use the fn_emit_cr_a_t
  // function typedef.
  //
  // the pipeline_memory_access<> functions reference information
  // stored within the context. The system objects {ie. coordinate_t}
  // are returned as a shared pointer, hence the .get() at the end.
  // this could be simplified to:
  // pipeline_memory_access<coordinate_t *> but that would
  // confuse internal design.
  auto fn_visitors = overload_visitors_t{

      [&](fn_emit_cr_t fn) { fn(context->cr); },

      [&](fn_emit_context_t fn) { fn(context); },

      [&](fn_emit_cr_a_t fn) {
        fn(context->cr, context->pipeline_memory_access<coordinate_t>().get());
      },

      [&](fn_emit_layout_t fn) {
        fn(context->pipeline_memory_access<PangoLayout *>());
      },

      [&](fn_emit_layout_a_t fn) {
        fn(context->pipeline_memory_access<PangoLayout *>(),
           context->pipeline_memory_access<coordinate_t>().get());
      },

      [&](fn_emit_cr_layout_t fn) {
        fn(context->cr, context->pipeline_memory_access<PangoLayout *>());
      },

      [&](std::monostate) {}};

  // arrange pipeline if necessary
  if (!bfinalized)
    pipeline_finalize();

  // if the pipeline is in a ready state.
  // providing broad functionality allow for expansion if necessary.
  if (pipeline_ready()) {
    // pipeline has already been sorted.
    for (auto o : pipeline_io) {
      auto overloaded_function = std::get<fn_emit_overload_t>(o);
      std::visit(fn_visitors, overloaded_function);
    }
  }
}
