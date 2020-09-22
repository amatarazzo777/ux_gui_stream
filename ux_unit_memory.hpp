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
\file uxunitmemory.hpp
\date 9/19/20
\version 1.0
\brief unit memory storage

*/

/**
\internal
\class unit_memory_storage_t
\brief class exposes template member functions in that class that it is
inherited in. The mechanism provides typed index storage of objects that may be
referenced at any time.

*/
namespace uxdevice {
typedef std::function<std::size_t(void)> hash_function_t;

typedef std::function<void()> pipeline_function_t;
typedef std::list<pipeline_function_t> pipeline_t;

typedef std::variant<std::monostate, pango_visitor_t,
                     cairo_coordinate_visitor_t>
    visitor_t;

typedef std::list<visitor_t> visitors_t;

class unit_memory_object_t {
public:
  std::any object = {};
  hash_function_t hash_function = {};
  const int pipeline_stage;
};

typedef std::unordered_map<std::type_index, unit_memory_object_t>
    unit_memory_unordered_map_t;

class display_unit_t;
template <typename ATTR> class unit_memory_storage_t {
public:
  unit_memory_storage_t() {}
  virtual ~unit_memory_storage_t() {}

  /**
    /fn unit_memory

  */
  template <typename T> void unit_memory(const std::shared_ptr<T> ptr) {
    auto ti = std::type_index(typeid(T));

    auto
        // place into unordered map
        storage[ti] = unit_memory_object_t {
      ptr, [&]() { return ptr->hash_code(); }, T::pipeline_stage
    }
  };
}
} // namespace uxdevice

/**
  \fn unit_memory_to_staged_pipeline
  \param overload_visitors_t &visitors
  \brief traverses the objects related and emits them
   to the overloaded provided.
*/
template <typename... Args>
void
unit_memory_to_staged_pipeline(pipeline_t &pipeline_io, Args &... args) {
  std::list pipeline_ret = {};
  visitors_t &overloaded_visitors = {args...};

  // Using lambda to compare elements.
  auto stage_comparison = [](int p1_stage, int p2_stage) {
    return p1_stage < p2_stage;
  };
  std::priority_queue<int, std::vector<pipeline_function_t>,
                      decltype(stage_comparison)>
      staged_pipeline(stage_comparison);

  // gather and add all associated items  with interfaces that match.
  // inserting them into the priority queue establishes order in the stages.
  // Each of the objects publicly inherit template class pipeline_sort_order
  // with a constant which is used also here within the priorty queue insertion.
  for (auto v : overloaded_visitors) {

    // iterate the storage,
    // if the interface for the objects match for visitation,
    // build and add a pipeline function for it. Here v is the visitor
    // function passed in which has the parameter information referenced
    // from the lambda function.
    for (o : storage)
      if (v.interface_type == o.interface_type)
        staged_pipeline.push_back(pipeline_function_t{v});
  }

  // unroll the queue into the io parameter sent.
  // the necessary drawing commands in the proper order will be
  // within the pipeline.
  while (!staged_pipeline.empty()) {
    pipeline_io.push_back(staged_pipeline.top());
    staged_pipeline.pop();
  }

  return;
}

/// @brief copy assignment operator
unit_memory_storage_t &operator=(const unit_memory_storage_t &other) {
  storage = other.storage;
  return *this;
}
/// @brief move assignment
unit_memory_storage_t &operator=(unit_memory_storage_t &&other) noexcept {
  storage = std::move(other.storage);
  return *this;
}

/// @brief move constructor
unit_memory_storage_t(unit_memory_storage_t &&other) noexcept
    : storage(std::move(other.storage)) {}

/// @brief copy constructor
unit_memory_storage_t(const unit_memory_storage_t &other)
    : storage(other.storage) {}

void unit_memory_linkages(display_context_t &other);

template <typename T>
void unit_memory(const std::shared_ptr<display_unit_t> ptr) {
  auto ti = std::type_index(typeid(T));
  storage[ti] = unit_memory_object_t{
      std::dynamic_pointer_cast<T>(ptr),
      [&]() { return std::dynamic_pointer_cast<T>(ptr)->hash_code(); }};
}

template <typename T>
auto unit_memory(void) const noexcept -> const std::shared_ptr<T> {
  std::shared_ptr<T> ptr = {};
  auto ti = std::type_index(typeid(T));
  auto item = storage.find(ti);
  if (item != storage.end()) {
    auto obj_data = item->second.object;
    ptr = std::any_cast<std::shared_ptr<T>>(obj_data);
  }
  return ptr;
}

template <typename T> void unit_memory_erase(void) {
  storage.erase(std::type_index(typeid(T)));
}

std::size_t unit_memory_hash_code(void) const noexcept {
  std::size_t value = {};
  for (auto &n : storage) {
    hash_combine(value, n.second.hash_function());
  }
  return value;
}

/**
\fn unit_memory_hash_code
\tparam T
*/
template <typename T> std::size_t unit_memory_hash_code(void) {
  std::size_t value = {};
  auto ti = std::type_index(typeid(T));
  auto item = storage.find(ti);
  if (item != storage.end()) {
    value = item->second.hash_function();
  }
  return value;
}

/**
\fn unit_memory_clear
\brief
*/
void unit_memory_clear(void) { storage.clear(); }

private:
unit_memory_unordered_map_t storage = {};
}
; // namespace uxdevice

} // namespace uxdevice
