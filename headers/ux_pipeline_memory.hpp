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

/*
 * \class pipeline_memory_object_t
 */
class pipeline_memory_object_t {
public:
  std::any object = {};
  hash_function_t hash_function = {};
  std::size_t pipeline_stage = {};
};

typedef std::unordered_map<std::type_index, pipeline_memory_object_t>
    pipeline_memory_unordered_map_t;

class display_unit_t;
/**
 * \class
 */
template <typename ATTR> class pipeline_memory_t {
public:
  pipeline_memory_t() {}
  virtual ~pipeline_memory_t() {}

  virtual void pipeline_acquire(cairo_t *cr, coordinate *a);
  virtual bool pipeline_has_required_linkages(void);

  /// @brief copy assignment operator
  pipeline_memory_t &operator=(const pipeline_memory_t &other) {
    storage = other.storage;
    return *this;
  }
  /// @brief move assignment
  pipeline_memory_t &operator=(pipeline_memory_t &&other) noexcept {
    storage = std::move(other.storage);
    return *this;
  }

  /// @brief move constructor
  pipeline_memory_t(pipeline_memory_t &&other) noexcept
      : storage(std::move(other.storage)) {}

  /// @brief copy constructor
  pipeline_memory_t(const pipeline_memory_t &other) : storage(other.storage) {}

  void pipeline_memory_linkages(display_context_t &other);

  template <typename T> void pipeline_disable_visit(void) {
    auto ti = std::type_index(typeid(T));
    if (ti != storage.end())
      storage.erase(ti);
  }

  /**
   /fn unit_memory

   */
  template <typename T>
  void pipeline_memory_store(const std::shared_ptr<T> ptr) {
    auto ti = std::type_index(typeid(T));

    // place into unordered map
    storage[ti] = pipeline_memory_object_t{
        ptr, [&]() { return ptr->hash_code(); }, ptr->pipeline_stage};
  }

  template <typename T>
  void pipeline_memory_store(const std::shared_ptr<display_unit_t> ptr) {
    auto ti = std::type_index(typeid(T));
    storage[ti] = pipeline_memory_object_t{
        std::dynamic_pointer_cast<T>(ptr),
        [&]() { return std::dynamic_pointer_cast<T>(ptr)->hash_code(); }};
  }

  template <typename T>
  auto pipeline_memory_access(void) const noexcept -> const std::shared_ptr<T> {
    std::shared_ptr<T> ptr = {};
    auto ti = std::type_index(typeid(T));
    auto item = storage.find(ti);
    if (item != storage.end()) {
      auto obj_data = item->second.object;
      ptr = std::any_cast<std::shared_ptr<T>>(obj_data);
    }
    return ptr;
  }

  template <typename T> void pipeline_memory_reset(void) {
    storage.erase(std::type_index(typeid(T)));
  }

  std::size_t pipeline_memory_hash_code(void) const noexcept {
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
  template <typename T> std::size_t pipeline_memory_hash_code(void) {
    std::size_t value = {};
    auto ti = std::type_index(typeid(T));
    auto item = storage.find(ti);
    if (item != storage.end()) {
      value = item->second.hash_function();
    }
    return value;
  }

  /**
   \fn pipeline_push
   \param overload_visitors_t &visitors
   \brief
   */
  template <std::size_t N> void pipeline_push(pipeline_fn_t &fn) {
    pipeline_io.emplace_back({N, fn});
  }

  /**
   \fn pipeline_push
   \param overload_visitors_t &visitors
   \brief
   */
  bool pipeline_ready(void) { return !pipeline_io.empty() }

  void pipeline_execute(void) {
    for (auto fn : pipeline_io)
      fn();
  }

  pipeline_finalize

      /**
       \fn unit_memory_to_staged_pipeline
       \param overload_visitors_t &visitors
       \brief traverses the objects related and emits them
       to the overloaded provided.
       */
      typedef std::pair<int, pipeline_function_t>
          pipeline_stage_object_t;
  template <typename... Args> void pipeline_push_visit(Args &... args) {
    visitors_t &overloaded_visitors = {args...};

    // Using lambda to compare elements.
    // this function establishes where in the sort order the items are placed.
    struct stage_comparison {
      bool operator()(pipeline_stage_object_t const &p1,
                      pipeline_stage_object_t const &p2) {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return std::get<0>(p1) < std::get<0>(p2);
      }
    };

    std::priority_queue<pipeline_stage_object_t,
                        std::vector<pipeline_stage_object_t>, stage_comparison>
        staged_pipeline(stage_comparison);

    // gather and add all associated items  with interfaces that match.
    // inserting them into the priority queue establishes order in the stages.
    // Each of the objects publicly inherit template class pipeline_sort_order
    // with a constant which is used also here within the priority queue
    // insertion.
    for (auto v : overloaded_visitors) {
      int polytype = {};

      if (std::holds_alternative<pango_visitor_t>(v)) {
        polytype = 1;
      } else if (std::holds_alternative<cairo_coordinate_visitor_t>(v)) {
        polytype = 2;
      }

      // iterate the storage,
      // if the interface for the objects match for visitation,
      // build and add a pipeline function for it. Here v is the visitor
      // function passed in which has the parameter information referenced
      // from the lambda function.
      for (auto o : storage) {

        if (polytype == o.second.pipeline_stage)

          staged_pipeline.push_back({polytype, v});
      }
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

  /**
   \fn unit_memory_clear
   \brief
   */

  void pipline_finalize(void) {

    // Using lambda to compare elements.
    // this function establishes where in the sort order the items are placed.
    struct less_than_key {
      bool operator()(pipeline_stage_object_t const &p1,
                      pipeline_stage_object_t const &p2) {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return std::get<0>(p1) < std::get<0>(p2);
      }
    };

    std::sort(storage.begin(), storage.end(), less_than_key());
  }

  /**
   \fn unit_memory_clear
   \brief
   */
  void pipeline_memory_reset(void) { storage.clear(); }

private:
  pipeline_memory_t storage = {};
  pipeline_t &pipeline_io = {};
};
} // namespace uxdevice
