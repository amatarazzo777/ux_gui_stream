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
 * with the pipeline_memory_t template.
 */

#include <base/inc/object_base.h>

/// @brief move constructor
uxdevice::pipeline_memory_t::pipeline_memory_t(
    pipeline_memory_t &&other) noexcept
    : bfinalized(std::move(other.bfinalized)),
      storage(std::move(other.storage)),
      pipeline_io(std::move(other.pipeline_io)) {}

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

/// @brief copy assignment operator
uxdevice::pipeline_memory_t &
uxdevice::pipeline_memory_t::operator=(const pipeline_memory_t &other) {
  bfinalized = other.bfinalized;
  storage = other.storage;
  pipeline_io = other.pipeline_io;
  return *this;
}

/// @brief move assignment
uxdevice::uxdevice::pipeline_memory_t &
uxdevice::pipeline_memory_t::operator=(pipeline_memory_t &&other) noexcept {
  bfinalized = std::move(other.bfinalized);
  storage = std::move(other.storage);
  pipeline_io = std::move(other.pipeline_io);
  return *this;
}

/**
 * @internal
 * @fn pipeline_ready
 * @brief determines if the pipeline is ready for traversal.
 */
bool uxdevice::pipeline_memory_t::pipeline_ready(void) {
  return bfinalized && !pipeline_io.empty();
}

/**
 * @fn pipeline_finalize
 * @brief prepares the pipeline memory for sequential execution by sorting.
 */
void uxdevice::pipeline_memory_t::pipeline_finalize(void) {

  if (bfinalized)
    return;
  /**
   * @struct
   * @brief this structure establishes where in the sort order the items are
   * placed. return "true" if "p1" is ordered before "p2", for example:
   */
  struct less_than_key {
    bool operator()(pipeline_io_storage_t const &p1,
                    pipeline_io_storage_t const &p2) {

      return std::get<std::size_t>(p1) < std::get<std::size_t>(p2);
    }
  };

  std::sort(pipeline_io.begin(), pipeline_io.end(), less_than_key());
  bfinalized = true;
}

/**
 * @internal
 * @fn pipeline_acquisition_t::pipeline_visit
 * @brief The function visits the pipeline sequentially executing the pipeline
 * lambdas using the context parameter. Parameters for visitors use the
 * pipeline_memory_access routine and pass the type requested to the public
 * member of the object.
 * */
void uxdevice::pipeline_memory_t::pipeline_visit(display_context_t *context) {

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
  static overload_visitors_t fn_visitors = overload_visitors_t{
      [&](const fn_emit_context_t &fn) { fn(context); },

      [&](const fn_emit_cr_t &fn) {
        context->window_manager->draw_fn([&](auto cr) { fn(cr); });
      },

      [&](const fn_emit_cr_a_t &fn) {
        std::lock_guard lock(pipeline_memory_mutex<coordinate_t>());
        context->window_manager->draw_fn([&](auto cr) {
          fn(cr, context->pipeline_memory_access<coordinate_t>().get());
        });
      },

      [&](const fn_emit_cr_layout_t &fn) {
        std::lock_guard lock(pipeline_memory_mutex<PangoLayout *>());
        context->window_manager->draw_fn([&](auto cr) {
          fn(cr, context->pipeline_memory_access<PangoLayout *>());
        });
      },

      [&](const fn_emit_layout_t &fn) {
        std::lock_guard lock(pipeline_memory_mutex<PangoLayout *>());
        fn(context->pipeline_memory_access<PangoLayout *>());
      },

      [&](const fn_emit_layout_a_t &fn) {
        std::scoped_lock lock(pipeline_memory_mutex<PangoLayout *>(),
                              pipeline_memory_mutex<coordinate_t>());
        fn(context->pipeline_memory_access<PangoLayout *>(),
           context->pipeline_memory_access<coordinate_t>().get());
      },

      [&](std::monostate) {}};

  // arrange pipeline if necessary
  pipeline_finalize();

  // if the pipeline is in a ready state.
  // providing broad functionality allow for expansion if necessary.
  if (pipeline_ready())
    for (auto o : pipeline_io)
      std::visit(fn_visitors, std::get<fn_emit_overload_t>(o));
}

/**
 * @internal
 * @fn pipeline_memory_hash_code
 * @brief returns the combined hash code for the entire pipeline memory.
 */
std::size_t
uxdevice::pipeline_memory_t::pipeline_memory_hash_code(void) const noexcept {
  std::size_t value = {};
  for (auto &n : storage)
    hash_combine(value, n.second.hash_function());

  return value;
}

/**
 * @fn std::size_t pipeline_fn_sequence(const visitor_interface_t*)
 * @brief builds the sequence number from a visitor interface object
 *
 * @param v
 * @return
 */
std::size_t uxdevice::pipeline_memory_t::pipeline_fn_sequence(
    const uxdevice::visitor_interface_t *v) {

  auto ti = std::visit([](auto &&arg) { return std::type_index{typeid(arg)}; },
                       v->fn);

  auto it = pipeline_fn_sequence_storage.find(ti);

  std::size_t ret = {};

  if (it == pipeline_fn_sequence_storage.end()) {
    pipeline_fn_sequence_storage[ti] = 1;
    ret = 1;
  } else {
    ret = pipeline_fn_sequence_storage[ti]++;
  }

  /**@brief modularize to index bit size leaves 2^8 objects for each
   * pipeline stage.*/
  ret = v->pipeline_order << 8 | (0xFF & ret);

  return ret;
}

/**
 * @internal
 * @fn pipeline_memory_clear
 * @brief A distinct function from the pipeline_memory_erase<T> where
 * individual items are erased. This function is exptect to be called during
 * a pipeline reset or clear for initialization. It also clears the function
 * index sequence associated with visiting index sequence count.
 */
void uxdevice::pipeline_memory_t::pipeline_memory_clear(void) {
  storage.clear();
  pipeline_fn_sequence_storage.clear();
}
