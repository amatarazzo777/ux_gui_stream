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
 * @author Anthony Matarazzo
 * @file ux_visitor_interface.h
 * @date 9/23/20
 * @version 1.0
 * @details This file encapsulates the definition of visitor interfaces
 * for each of the objects. There are several pieces that fit together, however
 * the system is dynamic in that new visitor interface prototypes can be
 * easily added. These are expressed in typedefs starting with the prefix "fn_".
 * There is a distinct oddity in that each prototype must be unique in its
 * parameters and types. The std::variant is used to combine all the interfaces
 * into an adapting storage container. The std::visit function operates to
 * invoke each of these if necessary given the parameters.
 *
 * The format fn_NAME_P1_P2_t is used to provide concise ordering as there are
 * multiple ones and of course the names are used repeatedly in the object
 * factory template definition of units. For example, an interface accepting a
 * cairo graphics pointer is named "fn_emit_cr_t" as "cr" is the common name
 * appearing in most sources and texts.
 *
 * These typedefs also operate as signatures using the std::type_index to store
 * an object instance that is bound to the function and an ordering numeric. The
 * ability to sort this is useful to a visitation pass/ Typically the
 * functionality of a display unit has multiple different effects on the
 * rendering pipeline. That is an object may inject into the textual layout
 * object as well as output graphic information to cairo using the pango-cairo
 * api.
 *
 *
 */

namespace uxdevice {

/**
 * forward class declarations.
 */
class system_base_t;
class display_context_t;
class coordinate_t;

/**
 * @brief function types that a display unit may accept or expose. These work in
 * input and out fashion. As well the types are used in conjunction with the
 * type index to identify it within the accepted_interfaces unordered map. */

/**
 * @internal
 * @typedef fn_emit_cr_t
 * @brief emit cairo_t *
 */
typedef std::function<void(cairo_t *)> fn_emit_cr_t;

/**
 * @internal
 * @typedef fn_emit_cr_a_t
 * @brief emit cairo_t*, coordinate_t *
 */
typedef std::function<void(cairo_t *, coordinate_t *)> fn_emit_cr_a_t;

/**
 * @internal
 * @typedef fn_emit_context_t
 * @brief emit to the display context.
 */
typedef std::function<void(display_context_t *context)> fn_emit_context_t;

/**
 * @internal
 * @typedef fn_emit_layout_t
 * @brief emit to the pango layout
 */
typedef std::function<void(PangoLayout *)> fn_emit_layout_t;

/**
 * @internal
 * @typedef fn_emit_layout_a_t
 * @brief emit to the pango layout with a rectangular "area" coordinate tied.
 * The translation to pango units is done here.
 */
typedef std::function<void(PangoLayout *, coordinate_t *)> fn_emit_layout_a_t;

/**
 * @internal
 * @typedef fn_emit_cr_layout_t
 * @brief emit graphics and the layout
 */
typedef std::function<void(cairo_t *cr, PangoLayout *)> fn_emit_cr_layout_t;

/**
 * @internal
 * @typedef fn_emit_overload_t
 * @brief a variant which is used to hold the function. std::visit is used. See
 * the function in ux_pipeline_memory.cpp
 * (uxdevice::pipeline_memory_t::pipeline_visit)
 */
typedef std::variant<std::monostate, fn_emit_cr_t, fn_emit_cr_a_t,
                     fn_emit_context_t, fn_emit_layout_t, fn_emit_layout_a_t,
                     fn_emit_cr_layout_t>
  fn_emit_overload_t;

/**
 * @internal
 * @class visitor_interface_t
 * @brief holds information for the visitor interface. The function
 * bind_dispatch is immediately called after the object is created
 * from the top level to bind the instance of data with the function.
 * Objects that inherit this class should override the bind_dispatch
 * function to do this. It is passed a polymorphic system base class
 * pointer which can gain access to any part of the object vtable.
 */
class visitor_interface_t {
public:
  visitor_interface_t() {}
  virtual ~visitor_interface_t() {}

  /// @brief move constructor
  visitor_interface_t(visitor_interface_t &&other) noexcept
    : fn(std::move(other.fn)), pipeline_order(std::move(other.pipeline_order)) {
  }

  /// @brief copy constructor
  visitor_interface_t(const visitor_interface_t &other)
    : fn(other.fn), pipeline_order(other.pipeline_order) {}

  /// @brief copy assignment
  visitor_interface_t &operator=(const visitor_interface_t &other) {
    fn = other.fn;
    pipeline_order = other.pipeline_order;
    return *this;
  }

  /// @brief move assignment
  visitor_interface_t &operator=(const visitor_interface_t &&other) noexcept {
    fn = std::move(other.fn);
    pipeline_order = std::move(other.pipeline_order);
    return *this;
  }

  fn_emit_overload_t fn = {};
  std::size_t pipeline_order = {};
  virtual void bind_dispatch(system_base_t *ptr) = 0;
};

/**
 * @typedef accept_interfaces_storage_t
 * @brief map holds the visitor interface by the type index of its function
 * prototype or fn_NAME_P1_P2_t std::function signature.
 */
typedef std::unordered_map<std::type_index, visitor_interface_t *>
  accepted_interfaces_storage_t;

/**
 * @internal
 * @class accepted_interfaces_base_t
 * @brief Because it is not known to the system how interfaces
 * are used in the resulting object factory template, this class
 * enables a communication between the dynamic visitor types to
 * be published here. accepted_interfaces contains a type index
 * and a pointer to its vtable within the object.
 */
class accepted_interfaces_base_t {
public:
  accepted_interfaces_base_t() {}
  virtual ~accepted_interfaces_base_t() {}
  /// @brief move constructor
  accepted_interfaces_base_t(accepted_interfaces_base_t &&other) noexcept
    : accepted_interfaces(std::move(other.accepted_interfaces)) {}

  /// @brief copy constructor
  accepted_interfaces_base_t(const accepted_interfaces_base_t &other)
    : accepted_interfaces(other.accepted_interfaces) {}

  /// @brief copy assignment
  accepted_interfaces_base_t &
  operator=(const accepted_interfaces_base_t &other) {
    accepted_interfaces = other.accepted_interfaces;
    return *this;
  }

  /// @brief move assignment
  accepted_interfaces_base_t &
  operator=(const accepted_interfaces_base_t &&other) noexcept {
    accepted_interfaces = std::move(other.accepted_interfaces);
    return *this;
  }

  accepted_interfaces_storage_t accepted_interfaces = {};
};

/**
 * @internal
 * @class accepted_interfaces_t
 * @tparam ...
 * @brief class used within the unit declaration parameters
 * to note abstracts that are published. All constructors are executed
 * with an instance pointer of the interfaces object. The called constructors
 * are expected to place the template parameterized std::size_t constant
 * naming the pipeline sort order and an unbound function abstract into the
 * unordered map accepted interfaces. The function is left as std::monostate on
 * the constructor call. Later this is bound to the correct instance above this
 * layer in logic by the function init_dispatch. init_dispatch is pass the
 * system base class type.
 */
template <typename... Args>
class accepted_interfaces_t : public accepted_interfaces_base_t,
                              public Args... {
public:
  accepted_interfaces_t() : Args(this)... {}
};

/**
 * @internal
 * @class system_base_t
 * @brief
 */
class system_base_t {
public:
  system_base_t() {}
  virtual ~system_base_t() {}
  /**
   * @fn void init_dispatch(void)
   * @brief The function passes the system base class pointer
   * to all abstract interface classes. The pointer is used
   * typically where the "this" is required in member function
   * invocation using the std::bind. The resulting function
   * is stored in the std::variant.
   *
   */
  void init_dispatch(void) {
    auto patterns = dynamic_cast<accepted_interfaces_base_t *>(this);
    for (auto n : patterns->accepted_interfaces)
      n.second->bind_dispatch(this);

    visitor_dispatch_bound = true;
  }
  bool visitor_dispatch_bound = false;
};

/* @brief names the stages in the pipeline. Each of the abstract class template
 * parameters when they are mentioned on the unit definition should name one of
 * these within its parameter.
 */
const int order_init = 1;
const int order_layout_option = 2;
const int order_render_option = 3;

const int order_before_render = 4;
const int order_render = 5;
const int order_after_render = 6;
const int order_terminate = 7;

/**
 * @details the constructors of these abstract interface classes are called by
 * the visitor_interfaces_t param pack expansion. The interface position within
 * the unordered_map accepted_interfaces is reserved by setting the type id but
 * the function is uninitialized (std::monostate). */

/**
 * @internal
 * @class abstract_emit_context_t
 * @tparam std::size_t ORDER
 * @brief The ORDER is the pipeline stage.
 */
template <std::size_t ORDER>
class abstract_emit_context_t : visitor_interface_t {
public:
  abstract_emit_context_t() {}

  /**
   * @internal
   * @fn  abstract_emit_context_t(accepted_interfaces_base_t*)
   * @brief constructor providing publishing of the interface abstract while the
   * function binding to data is unknown. Each of these are called if inherited
   * using the visitor_interface_t<> template syntax within the unit factory
   * declaration.
   *
   * @param ptr
   */
  abstract_emit_context_t(accepted_interfaces_base_t *ptr) {
    pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(fn_emit_context_t))] = this;
  }

  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_context_t{std::bind(
      &abstract_emit_context_t::emit,
      dynamic_cast<abstract_emit_context_t *>(ptr), std::placeholders::_1)};
  }

  virtual ~abstract_emit_context_t() {}

  /// @brief move constructor
  abstract_emit_context_t(abstract_emit_context_t &&other) noexcept
    : visitor_interface_t(std::move(other)) {}

  /// @brief copy constructor
  abstract_emit_context_t(const abstract_emit_context_t &other)
    : visitor_interface_t(other) {}

  /// @brief copy assignment
  abstract_emit_context_t &operator=(const abstract_emit_context_t &other) {
    visitor_interface_t::operator=(other);
    return *this;
  }

  /// @brief move assignment
  abstract_emit_context_t &
  operator=(const abstract_emit_context_t &&other) noexcept {
    visitor_interface_t::operator=(other);
    return *this;
  }

  virtual void emit(display_context_t *context) = 0;
};

/**
 * @internal
 * @class abstract_emit_cr_t
 * @tparam std::size_t ORDER
 * @brief
 */
template <std::size_t ORDER> class abstract_emit_cr_t : visitor_interface_t {
public:
  abstract_emit_cr_t() {}
  abstract_emit_cr_t(accepted_interfaces_base_t *ptr) {
    pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(fn_emit_cr_t))] = this;
  }

  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_t{std::bind(&abstract_emit_cr_t::emit,
                                dynamic_cast<abstract_emit_cr_t *>(ptr),
                                std::placeholders::_1)};
  }

  virtual ~abstract_emit_cr_t() {}
  virtual void emit(cairo_t *cr) = 0;
};

/**
 * @internal
 * @class abstract_emit_cr_absolute_t
 * @tparam std::size_t ORDER
 * @brief
 */
template <std::size_t ORDER>
class abstract_emit_cr_absolute_t : visitor_interface_t {
public:
  abstract_emit_cr_absolute_t() {}
  abstract_emit_cr_absolute_t(accepted_interfaces_base_t *ptr) {
    pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(fn_emit_cr_t))] = this;
  }

  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_t{std::bind(
      &abstract_emit_cr_absolute_t::emit_absolute,
      dynamic_cast<abstract_emit_cr_absolute_t *>(ptr), std::placeholders::_1)};
  }

  virtual ~abstract_emit_cr_absolute_t() {}
  virtual void emit_absolute(cairo_t *cr) = 0;
};

/**
 * @internal
 * @class abstract_emit_cr_relative_t
 * @tparam std::size_t ORDER
 * @brief
 */
template <std::size_t ORDER>
class abstract_emit_cr_relative_t : visitor_interface_t {
public:
  abstract_emit_cr_relative_t() {}
  abstract_emit_cr_relative_t(accepted_interfaces_base_t *ptr) {
    pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(fn_emit_cr_t))] = this;
  }

  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_t{std::bind(
      &abstract_emit_cr_relative_t::emit_relative,
      dynamic_cast<abstract_emit_cr_relative_t *>(ptr), std::placeholders::_1)};
  }
  virtual ~abstract_emit_cr_relative_t() {}
  virtual void emit_relative(cairo_t *cr) = 0;
};

/**
 * @internal
 * @class abstract_emit_cr_a_t
 * @tparam std::size_t ORDER
 * @brief
 */
template <std::size_t ORDER> class abstract_emit_cr_a_t : visitor_interface_t {
public:
  abstract_emit_cr_a_t() {}
  abstract_emit_cr_a_t(accepted_interfaces_base_t *ptr) {
    pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(fn_emit_cr_a_t))] = this;
  }

  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_a_t{std::bind(
      &abstract_emit_cr_a_t::emit, dynamic_cast<abstract_emit_cr_a_t *>(ptr),
      std::placeholders::_1, std::placeholders::_2)};
  }
  virtual ~abstract_emit_cr_a_t() {}
  virtual void emit(cairo_t *cr, coordinate_t *a) = 0;
};

/**
 * @internal
 * @class abstract_emit_layout_t
 * @tparam std::size_t ORDER
 * @brief
 */
template <std::size_t ORDER>
class abstract_emit_layout_t : visitor_interface_t {
public:
  abstract_emit_layout_t() {}

  abstract_emit_layout_t(accepted_interfaces_base_t *ptr) {
    pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(fn_emit_layout_t))] = this;
  }

  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_layout_t{std::bind(&abstract_emit_layout_t::emit,
                                    dynamic_cast<abstract_emit_layout_t *>(ptr),
                                    std::placeholders::_1)};
  }

  virtual ~abstract_emit_layout_t() {}
  virtual void emit(PangoLayout *layout) = 0;
};

/**
 * @internal
 * @class abstract_emit_layout_a_t
 * @tparam std::size_t ORDER
 * @brief
 */
template <std::size_t ORDER>
class abstract_emit_layout_a_t : visitor_interface_t {
public:
  abstract_emit_layout_a_t() {}
  abstract_emit_layout_a_t(accepted_interfaces_base_t *ptr) {
    pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(fn_emit_layout_a_t))] =
      this;
  }

  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_layout_a_t{
      std::bind(&abstract_emit_layout_a_t::emit,
                dynamic_cast<abstract_emit_layout_a_t *>(ptr),
                std::placeholders::_1, std::placeholders::_2)};
  }

  virtual ~abstract_emit_layout_a_t(){};
  virtual void emit(PangoLayout *layout, coordinate_t *a) = 0;
};

/**
 * @internal
 * @class abstract_emit_cr_layout_t
 * @tparam std::size_t ORDER
 * @brief
 */
template <std::size_t ORDER>
class abstract_emit_cr_layout_t : visitor_interface_t {
public:
  abstract_emit_cr_layout_t() {}
  abstract_emit_cr_layout_t(accepted_interfaces_base_t *ptr) {
    pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(fn_emit_cr_layout_t))] =
      this;
  }

  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_layout_t{
      std::bind(&abstract_emit_cr_layout_t::emit,
                dynamic_cast<abstract_emit_cr_layout_t *>(ptr),
                std::placeholders::_1, std::placeholders::_2)};
  }

  virtual ~abstract_emit_cr_layout_t() {}
  virtual void emit(cairo_t *cr, PangoLayout *layout) = 0;
};

/**
 * @internal
 * @class visitor_base_t
 * @brief
 */
class visitor_base_t {
public:
  visitor_base_t() {}
  virtual ~visitor_base_t() {}
};

/**
 * @internal
 * @brief these unique bit signatures identify objects as being part of the
 * visitation. Therefore, the objects are included in the pipeline memory of an
 * pipeline memory object. Pipeline memory objects typically uses these for
 * rendering things using it as parameters (see pipeline_memory_access). As
 * well, these objects may have "emittable" visitation interfaces which are used
 * within the pipeline_visit function and also available to the internal
 * pipeline_acquire function. So these label the objects within the template
 * factory parameters of the unit.
 */
const std::size_t all_rendering_bits = 0b111111111111;
const std::size_t object_data_storage_bits = 0b0001;
const std::size_t textual_render_normal_bits = 0b0010;
const std::size_t textual_render_path_bits = 0b0100;
const std::size_t image_block_bits = 0b1000;

/**
 * @internal
 * @class visitor_targets_t
 * @brief stores a bit pattern consisting of all passed parameters
 *  | together. An object that inherits this class within the unit factory
 *  template declaration can be tested by the pipeline_memory_linkages function.
 *
 * @tparam Args
 */
class visitor_bits_t {
public:
  visitor_bits_t(std::size_t _bits) : associated_bits{_bits} {}
  virtual ~visitor_bits_t() {}
  const std::size_t associated_bits = {};
};

/**
 * @class visitor_targets_t
 * @brief reduces the bit patterns sent to an ored version at compile time.
 * This makes for a single movq at runtime.
 *
 * @tparam Args
 */
template <std::size_t... Args> class visitor_targets_t : public visitor_bits_t {
public:
  visitor_targets_t() : visitor_bits_t(combine_targets(Args...)) {}
  virtual ~visitor_targets_t() {}
  template <typename... TARGET_BITS>
  constexpr std::size_t combine_targets(TARGET_BITS const &&... target_bits) {
    return (... | target_bits);
  }
};

} // namespace uxdevice
