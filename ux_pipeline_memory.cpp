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
 * @file ux_pipeline_memory.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief The file contains template implementations of functions associated
 * with the pipeline_memory_t template. The file has a list of explicit template
 * instantiations for visitors that should be maintained if new visitors are
 * added. This provides the functionality that only units associated with the
 * visitor attribute are copied.
 */

/**
 * @internal
 * @fn unit_memory_linkages
 * @brief The function copies the applicable units from the context. The
 * link_visitor_target should be one of the visitor bit patterns defined in
 * ux_visitor_interface.h
 */

void uxdevice::pipeline_memory_t::pipeline_memory_linkages(
  display_context_t *context, std::size_t link_visitor_target) {
  for (auto n : context->storage)
    if (n.second.visitor_target_bits & link_visitor_target)
      storage[n.first] = n.second;
}

/**
 * @internal
 * @fn pipeline_acquisition_t::pipeline_execute
 * @brief The function visits the pipeline sequentially executing the pipeline
 * lambdas using the context parameter. Parameters for visitors use the
 * pipeline_memory_access routine and pass the type requested to the public
 * member of the object.
 * */
void uxdevice::pipeline_memory_t::pipeline_execute(display_context_t *context) {

  /** @brief All types have to be declared here in the visitor The compile
   * errors are very difficult to relate to this. Each of the std::functions
   * have to have a unique prototype signature. This is true even if the class
   * has a different name. Notice that the abstract class
   * abstract_emit_cr_relative_t and abstract_emit_cr_absolute_t use the
   * fn_emit_cr_a_t function typedef.  The pipeline_memory_access<> functions
   * reference information stored within the context. The system objects {ie.
   * coordinate_t} are returned as a shared pointer, hence the .get() at the
   * end. this could be simplified to: pipeline_memory_access<coordinate_t *>
   * but that would confuse internal design.  */
  auto fn_visitors = overload_visitors_t{

    [&](const fn_emit_cr_t &fn) { fn(context->cr); },

    [&](const fn_emit_context_t &fn) { fn(context); },

    [&](const fn_emit_cr_a_t &fn) {
      fn(context->cr, context->pipeline_memory_access<coordinate_t>().get());
    },

    [&](const fn_emit_layout_t &fn) {
      fn(context->pipeline_memory_access<PangoLayout *>());
    },

    [&](const fn_emit_layout_a_t &fn) {
      fn(context->pipeline_memory_access<PangoLayout *>(),
         context->pipeline_memory_access<coordinate_t>().get());
    },

    [&](const fn_emit_cr_layout_t &fn) {
      fn(context->cr, context->pipeline_memory_access<PangoLayout *>());
    },

    [&](std::monostate) {}};

  // arrange pipeline if necessary
  if (!bfinalized)
    pipeline_finalize();

  // if the pipeline is in a ready state.
  // providing broad functionality allow for expansion if necessary.
  if (pipeline_ready())
    for (auto o : pipeline_io) {
      auto overloaded_function = std::get<fn_emit_overload_t>(o);
      std::visit(fn_visitors, overloaded_function);
    }
}
