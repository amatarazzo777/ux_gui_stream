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
 @author Anthony Matarazzo
 @file ux_pipeline_memory.hpp
 @date 9/19/20
 @version 1.0
 @brief pipeline memory storage and visitation logic

 */

#pragma once

/**
 * @internal
 * @class unit_memory_storage_t
 * @brief class exposes template member functions in that class that it is
 * inherited in. The mechanism provides typed index storage of objects that may
 * be referenced at any time.
 */
namespace uxdevice {
typedef std::function<std::size_t(void)> hash_function_t;

/**
 * @internal
 * @typedef pipeline_io_storage_t
 * @brief pipeline storage object
 */

typedef std::tuple<std::size_t, fn_emit_overload_t> pipeline_io_storage_t;
/**
 * @internal
 * @typedef pipeline_t
 * @brief pipeline storage
 */
typedef std::vector<pipeline_io_storage_t> pipeline_t;

/**
 * @internal
 * @typedef pipeline_memory_storage_t
 *
 * @brief object storage in a typed index fashion
 */
typedef std::unordered_map<std::type_index, pipeline_memory_object_t>
    pipeline_memory_storage_t;

/**
 * @internal
 * @typedef
 * @brief holds index of function prototype usage for counting sequence within
 * the order stage group. The stage group id "order" constant is shifted << to
 * the left. This number is used in the lower bound of the overall index
 * sequence placed within the pipeline.
 */
typedef std::unordered_map<std::type_index, std::size_t>
    pipeline_fn_sequence_storage_t;

/**
 * @brief forward class declaration
 */
class display_unit_t;

/**
 * @internal
 * @class pipeline_memory_t
 * @brief Class that provides a storage and build template functions that
 * iterate based up function prototype signatures already defined. The
 * pipeline is sorted and a sequential execution of the underlying object
 * member functions is called using the std::visit for variant.
 *
 * The class, however named differently, accepted_interface_t is directly used
 * within the approach to building and using the objects in this fashion.
 * If interface signatures match up, they simply placed into the pipeline. The
 * order at this point is undetermined as the iterator used is over the
 * unordered_map. When items are inserted, along with the order, they also are
 * numerically labeled as a subset index.
 */
class pipeline_memory_t {
public:
  pipeline_memory_t();
  virtual ~pipeline_memory_t();

  /// @brief copy assignment operator
  pipeline_memory_t &operator=(const pipeline_memory_t &other);

  /// @brief move assignment
  pipeline_memory_t &operator=(pipeline_memory_t &&other) noexcept;

  /// @brief move constructor
  pipeline_memory_t(pipeline_memory_t &&other) noexcept;

  /// @brief copy constructor
  pipeline_memory_t(const pipeline_memory_t &other);

  /**
   * @internal
   * @fn void pipeline_acquire(void)=0
   * @brief Interface function must be implemented by object. The function
   * fills the pipeline using the visit method provided in template below.
   * An important feature is that single lambda or groups of lambda stored
   * within the pipeline can be inserted by signature prototype. Once sorted,
   * this provides a stream line execution of the function storage within the
   * vector.
   *
   */
  virtual void pipeline_acquire(void) = 0;

  /**
   * @internal
   * @fn bool pipeline_has_required_linkages(void)=0
   * @brief Interface function must be implemented by object. THe function
   * provides a validation routine before the object is trusted to render and
   * operate correctly with the parameter and environment. The object makes
   * the decision.
   *
   * @return
   */
  virtual bool pipeline_has_required_linkages(void) = 0;

  /**
   * @internal
   * @fn pipeline_memory_linkages
   * @brief function used by object during initialization. It provides a
   * summary copy view and shared pointer linkages to the items stored within
   * the context. The matching is done via @ operation within the
   * visitor_target passed. Simply the object internally names and decides
   * which set it may want a copy of. The routine iterates the collection and
   * builds a new one locally. Additionally other objects may be stored within
   * the pipeline that is used by the object elsewhere. This is done with the
   * lay out visitor. The object stored within the map by type is within an
   * std::any object so requesting it is done by updating the visitor
   * prototype and dispatch lambda.
   */
  void pipeline_memory_linkages(display_context_t *context,
                                std::size_t visitor_target);

  std::size_t pipeline_fn_sequence(const visitor_interface_t *v);
  void pipeline_memory_clear(void);

  /// @brief performs the sequence of functions
  void pipeline_visit(display_context_t *context);

  /// @brief determines if stream is available
  bool pipeline_ready(void);

  /// @brief sorts and optimizes
  void pipeline_finalize(void);

  /// @overload
  /// @brief entire memory hash, there is a template version for a specific
  /// item.
  std::size_t pipeline_memory_hash_code(void) const noexcept;

  /**
   * @fn pipeline_disable_visit
   * @tparam T
   */
  template <typename T> void pipeline_disable_visit(void) {
    storage.erase(std::type_index(typeid(T)));
  }

  /**
   * @internal
   * @fn pipeline_memory_store
   * @tparam T
   * @brief
   * Storage of a shared pointer with variations on system defined and user
   * input. This provides effective use of std::any and facilitates the
   * storage of visitor parameter data with the ability to expand to other
   * data types. The usefulness of the using for type alias provides effective
   * means of expansion.
   */
  template <typename T>
  void pipeline_memory_store(const std::shared_ptr<T> ptr) {
    auto ti = std::type_index(typeid(T));
    std::size_t _associated_bits = object_data_storage_bits;
    accepted_interfaces_storage_t *_accepted_interfaces = {};
    hash_function_t _hash_fn = {};

    /** @brief the target visitor pattern the object supports. the default is
     * object_data_storage_bits. These are linkages used by the object
     * internally at visitation during pipeline_acquire.
     * The system ignore these during pipeline_memory_linkages. */
    if constexpr (std::is_base_of<visitor_bits_t, T>::value)
      _associated_bits = ptr->associated_bits;

    /** @brief if the object has accepted interfaces class, it has published
     * visitor members.*/
    if constexpr (std::is_base_of<accepted_interfaces_base_t, T>::value)
      _accepted_interfaces = &ptr->accepted_interfaces;

    /** @brief object supports a hashing interface*/
    if constexpr (std::is_base_of<hash_members_t, T>::value)
      _hash_fn = [&]() { return ptr->hash_code(); };

    /** @brief place into unordered map as a visitor object shared_ptr */
    storage[ti] = pipeline_memory_object_t{ptr, _associated_bits,
                                           _accepted_interfaces, _hash_fn};
  }

  /**
   * @internal
   * @fn pipeline_memory_store
   * @tparam T
   * @brief Storage of a just data can be any type but only one type position
   * exists for it. This is a visitor parameter the internal system will skip
   * over when processing unless named within the visitor or used within one
   * of the underlying lambda functions within the pipeline.
   */
  template <typename T> void pipeline_memory_store(const T &o) {
    auto ti = std::type_index(typeid(T));
    storage[ti] = pipeline_memory_object_t{o, object_data_storage_bits, nullptr,
                                           [&]() { return ti.hash_code(); }};
  }

  /**
   * @fn  pipeline_memory_mutex(void)
   * @brief used to access the memory during a visit. When the object is
   * stored, a mutex is created for it.
   *
   * @tparam T
   * @return
   */
  template <typename T> std::mutex &pipeline_memory_mutex(void) {
    std::shared_ptr<T> ptr = {};
    auto ti = std::type_index(typeid(T));
    auto item = storage.find(ti);
    if (item == storage.end())
      throw std::runtime_error(
          "pipeline_memory_mutex accessed before value is initialized. ");

    return item->second.data_mutex;
  }

  /**
   * @internal
   * @fn pipeline_memory_access
   * @tparam T
   * @brief returns the shared pointer of the visitor or just the data if it
   * is not an internal system processing type. That is system processing
   * types are deduced by their inheritance of the  visitor_interfaces_t
   * within this programming construct. The return type is deduced using the
   * decltype(auto) while the type changes based upon the constexpr if. *
   *
   * TODO build mutex support for some objects, is needed here? class of
   * double with mutex support and signals for all values system wide?.
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
   * @internal
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
   * @internal
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
   * @fn std::size_t pipeline_fn_sequence(void)
   * @brief maintains unordered map count associated by type index of the
   * template parameter. It is initialized to 1 if not found, however
   * incremented if exists.
   * @tparam N - stage.
   * @tparam FN
   * @return the index sequence.
   */
  template <std::size_t N, typename FN> std::size_t pipeline_fn_sequence(void) {
    auto ti = std::type_index(typeid(FN));
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
    ret = N << 8 | (0xFF & ret);

    return ret;
  }

  /**
   * @internal
   * @fn pipeline_push
   * @param overload_visitors_t &visitors
   * @brief places a lambda expression onto the pipeline with the order
   * attached. later the order is used to sort the contents.
   */
  template <std::size_t N, typename FN> void pipeline_push(FN fn) {
    pipeline_io.emplace_back(
        std::make_tuple(pipeline_fn_sequence<N, FN>(), fn));
    bfinalized = false;
  }

  /**
   * @internal
   * @fn pipeline_push_visit
   * @tparam Args... the caller simply names the fn_emit_SIGNATURES for the name
   * of match to emit within the pipeline at this time. sequencing of the
   * various stages are maintained.
   * @brief traverses the objects related and emits them to the overloaded
   * provided.
   */
  template <typename... Args> void pipeline_push_visit(void) {
    const static std::list<std::type_index> overloaded_visitors = {
        std::type_index(typeid(Args))...};

    /**
     * @brief Gather and add all associated items  with interfaces that match.
     * inserting them into the priority queue establishes order in the stages.
     * Each of the objects publicly inherit template class pipeline_sort_order
     * with a constant which is used also here within the priority queue
     * insertion.  iterate the storage, if the interface for the objects match
     * for visitation, build and add a pipeline function for it. Here v is the
     * visitor function passed in which has the parameter information
     * referenced from the lambda function.
     *
     * The loop process:
     *     For each of the visitors passed.. (auto ti)
     *      go through all of the "accept" objects (auto o)
     *      within pipeline memory storage.
     *      if interface is supported, add to pipeline. (v=o.second)
     *
     */
    for (auto ti : overloaded_visitors)
      /** @brief this part of the mechanism can be increased by combining hash
       * keys of the dual information required. todo add an unordered map of
       * storage index. */
      for (auto o : storage)
        /** @brief this subtle logic uses an operator overload within the
         * o.second[] call. This search the object for the fn_emit_SIGNATURE by
         * type index. Essentially encapsulating the find against the accepted
         * method.*/
        if (auto v = o.second[ti])
          pipeline_io.emplace_back(
              std::make_tuple(pipeline_fn_sequence(v), v->fn));

    // ensure this will be sorted before executed.
    bfinalized = false;
  }

  /// @brief public variables
  bool bfinalized = false;
  pipeline_memory_storage_t storage = {};
  pipeline_fn_sequence_storage_t pipeline_fn_sequence_storage = {};
  pipeline_t pipeline_io = {};
}; // namespace uxdevice
} // namespace uxdevice
