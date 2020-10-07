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
 @author Anthony Matarazzo
 @file ux_pipeline_memory.hpp
 @date 9/19/20
 @version 1.0
 @brief pipeline memory storage and visitation logic

 */

/**
 * @internal
 * @class unit_memory_storage_t
 * @brief class exposes template member functions in that class that it is
 * inherited in. The mechanism provides typed index storage of objects that may
 * be referenced at any time.
 */
namespace uxdevice {
typedef std::function<std::size_t(void)> hash_function_t;

typedef std::tuple<std::size_t, fn_emit_overload_t> pipeline_io_storage_t;
typedef std::vector<pipeline_io_storage_t> pipeline_t;

/*
 * @internal
 * @class pipeline_memory_object_t
 *
 * @brief object storage
 */
class pipeline_memory_object_t {
public:
  std::any object = {};
  std::unordered_map<std::type_index, visitor_interface_t *>
      *accept_interfaces = {};
  hash_function_t hash_function = {};
};

/*
 * internal
 * @class pipeline_memory_object_t
 *
 * @brief object storage
 */
typedef std::unordered_map<std::type_index, pipeline_memory_object_t>
    pipeline_memory_unordered_map_t;

/**
 * forward class declaration
 */
class display_unit_t;

/**
 * @internal
 * @class pipeline_acquisition_t
 * @brief
 */
class pipeline_acquisition_t {
public:
  pipeline_acquisition_t() {}
  virtual ~pipeline_acquisition_t() {}

  /// @brief copy assignment operator
  pipeline_acquisition_t &operator=(const pipeline_acquisition_t &other) {
    bfinalized = other.bfinalized;
    storage = other.storage;
    pipeline_io = other.pipeline_io;
    return *this;
  }
  /// @brief move assignment
  pipeline_acquisition_t &operator=(pipeline_acquisition_t &&other) noexcept {
    bfinalized = std::move(other.bfinalized);
    storage = std::move(other.storage);
    pipeline_io = std::move(other.pipeline_io);
    return *this;
  }

  /// @brief move constructor
  pipeline_acquisition_t(pipeline_acquisition_t &&other) noexcept
      : bfinalized(std::move(other.bfinalized)),
        storage(std::move(other.storage)),
        pipeline_io(std::move(other.pipeline_io)) {}

  /// @brief copy constructor
  pipeline_acquisition_t(const pipeline_acquisition_t &other)
      : bfinalized(other.bfinalized), storage(other.storage),
        pipeline_io(other.pipeline_io) {}

  virtual void pipeline_acquire(cairo_t *cr, coordinate_t *a) = 0;
  virtual bool pipeline_has_required_linkages(void) = 0;
  void pipeline_execute(display_context_t *context);

  /**
   * @internal
   * @fn pipeline_ready
   * @brief determines if the pipeline is ready for traversal.
   */
  bool pipeline_ready(void) { return !pipeline_io.empty(); }

  /**
   * @fn pipeline_finalize
   * @brief prepares the pipeline memory for sequential execution by sorting.
   */
  void pipeline_finalize(void) {

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

  bool bfinalized = false;
  pipeline_memory_unordered_map_t storage = {};
  pipeline_t pipeline_io = {};
};

/**
 * @class pipeline_memory_t
 */
template <typename ATTR>
class pipeline_memory_t : public pipeline_acquisition_t {
public:
  pipeline_memory_t() {}
  virtual ~pipeline_memory_t() {}

  /// @brief copy assignment operator
  pipeline_memory_t &operator=(const pipeline_memory_t &other) {
    pipeline_acquisition_t::operator=(other);
    return *this;
  }
  /// @brief move assignment
  pipeline_memory_t &operator=(pipeline_memory_t &&other) noexcept {
    pipeline_acquisition_t::operator=(other);
    return *this;
  }

  /// @brief move constructor
  pipeline_memory_t(pipeline_memory_t &&other) noexcept
      : pipeline_acquisition_t(other) {}

  /// @brief copy constructor
  pipeline_memory_t(const pipeline_memory_t &other)
      : pipeline_acquisition_t(other) {}

  /**
   * @fn pipeline_memory_linkages
   */
  void pipeline_memory_linkages(display_context_t *context);

  /**
   * @fn pipeline_disable_visit
   * @tparam T
   */
  template <typename T> void pipeline_disable_visit(void) {
    storage.erase(std::type_index(typeid(T)));
  }

  /**
   * @fn pipeline_memory_store
   * @tparam T
   * @brief
   * Storage of a shared pointer with variations on system defined and user
   * input. This provides effective use of std::any and facilitates the storage
   * of visitor parameter data with the ability to expand to other data types.
   * The usefulness of the using for type alias provides effective means of
   * expansion.
   */
  template <typename T>
  void pipeline_memory_store(const std::shared_ptr<T> ptr) {
    auto ti = std::type_index(typeid(T));

    // place into unordered map as a visitor object shared_ptr
    if constexpr (std::is_base_of<visitor_interfaces_base_t, T>::value) {
      storage[ti] = pipeline_memory_object_t{
          ptr, &ptr->accepted_interfaces, [&]() { return ptr->hash_code(); }};

      // place into umap as a hashing visitor parameter shared_ptr
    } else if constexpr (std::is_base_of<hash_members_t, T>::value) {
      storage[ti] = pipeline_memory_object_t{
          ptr, nullptr, [&]() { return ptr->hash_code(); }};

    } else {
      // just data as a visitor parameter  shared_ptr
      storage[ti] = pipeline_memory_object_t{ptr, nullptr,
                                             [&]() { return ti.hash_code(); }};
    }
  }

  /**
   * @fn pipeline_memory_store
   * @tparam T
   * @brief Storage of a just data can be any type but only one type position
   * exists for it. This is a visitor parameter the internal system will skip
   * over when processing unless named within the visitor or used within one of
   * the underlying lambda functions within the pipeline.
   */
  template <typename T> void pipeline_memory_store(const T &o) {
    auto ti = std::type_index(typeid(T));
    storage[ti] =
        pipeline_memory_object_t{o, nullptr, [&]() { return ti.hash_code(); }};
  }

  /**
   * @fn pipeline_memory_access
   * @tparam T
   * @brief returns the shared pointer of the visitor or just the data if it is
   * not an internal system processing type. That is system processing types are
   * deduced by their inheritance of the  visitor_interfaces_t within this
   * programming construct. The return type is deduced using the decltype(auto)
   * while the type changes based upoon the constexpr if. *
   */
  template <typename T>
  decltype(auto) pipeline_memory_access(void) const noexcept {
    if constexpr (std::is_base_of<display_unit_t, T>::value) {
      std::shared_ptr<T> ptr = {};
      auto ti = std::type_index(typeid(T));
      auto item = storage.find(ti);
      if (item != storage.end()) {
        auto obj_data = item->second.object;
        ptr = std::any_cast<std::shared_ptr<T>>(obj_data);
      }
      return ptr;
    } else {
      T _data = {};
      auto ti = std::type_index(typeid(T));
      auto item = storage.find(ti);
      if (item != storage.end()) {
        auto obj_data = item->second.object;
        _data = std::any_cast<T>(obj_data);
      }
      return _data;
    }
  }

  /**
   * @fn pipeline_memory_reset
   * @tparam T
   * @brief resets the memory associated with the type passed in the template
   * parameter.
   */
  template <typename T> void pipeline_memory_reset(void) {
    storage.erase(std::type_index(typeid(T)));
    bfinalized = false;
  }
  /**
   * @fn pipeline_memory_hash_code
   * @brief returns the combined hash code for the entire pipeline memory.
   */
  std::size_t pipeline_memory_hash_code(void) const noexcept {
    std::size_t value = {};
    for (auto &n : storage) {
      hash_combine(value, n.second.hash_function());
    }
    return value;
  }

  /**
   * @fn pipeline_memory_hash_code
   * @tparam T
   * @brief returns the hash code of a specific type.
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
   * @fn pipeline_push
   * @param overload_visitors_t &visitors
   * @brief places a lambda expression onto the pipeline with the order
   * attached. later the order is used to sort the contents.
   */
  template <std::size_t N, typename FN> void pipeline_push(FN fn) {
    fn_emit_overload_t vfn = fn;
    pipeline_io.emplace_back(std::make_tuple(N, vfn));
    bfinalized = false;
  }

  /**
   * @fn unit_memory_to_staged_pipeline
   * @param overload_visitors_t &visitors
   * @brief traverses the objects related and emits them to the overloaded
   * provided.
   */
  template <typename... Args> void pipeline_push_visit(void) {
    std::list<std::type_index> overloaded_visitors = {
        std::type_index(typeid(Args))...};

    /**
     * Gather and add all associated items  with interfaces that match.
     * inserting them into the priority queue establishes order in the stages.
     * Each of the objects publicly inherit template class pipeline_sort_order
     * with a constant which is used also here within the priority queue
     * insertion.  iterate the storage, if the interface for the objects match
     * for visitation, build and add a pipeline function for it. Here v is the
     * visitor function passed in which has the parameter information referenced
     * from the lambda function.*/

    // for each of the visitors passed.
    for (auto ti : overloaded_visitors)
      // go through all of the "accept" objects within pipeline memory
      // storage.
      for (auto o : storage)
        // test if object accepts the visitor interface
        // requested by the template param pack (ti)
        if (o.second.accept_interfaces) {
          auto v = o.second.accept_interfaces->find(ti);
          if (v != o.second.accept_interfaces->end())
            pipeline_io.push_back({v->second->pipeline_order, v->second->fn});
        }

    bfinalized = false;
  }

  /**
   * @fn unit_memory_clear
   * @brief
   */
  void pipeline_memory_reset(void) { storage.clear(); }
};
} // namespace uxdevice
