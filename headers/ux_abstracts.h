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
\file ux_abstracts.h
\date 9/23/20
\version 1.0
@details
 Abstract interfaces that should be defined within the class definition
template parameters of the object factories. When these are placed there, the
public members of the pure abstract class are inherited within the resulting
class generated  from the template. This allows one to implement the function
for callback or usage where appropriate. The uxdisplayunits.cpp file contains
the implementation of these objects. Objects are of a trivial storage or class
type.  These methods provide usage for the data or marking the position within
the display list.
*/



namespace uxdevice {

/**
 * forward class declarations.
 */
class system_base_t;
class display_context_t;
class coordinate_t;

/**
@brief function types that a display unit may accept or expose. These work
in input and out fashion. As well the types are used in conjunction with
the type index to identify it within the accepted_interfaces unordered 
map.
 */
 
/**
@internal
@typedef fn_emit_cr_t
@brief 
 */
typedef std::function<void(cairo_t *)> fn_emit_cr_t;

/**
@internal
@typedef fn_emit_cr_a_t
@brief 
 */
typedef std::function<void(cairo_t *, coordinate_t *)> fn_emit_cr_a_t;

/**
@internal
@typedef fn_emit_context_t
@brief 
 */
typedef std::function<void(display_context_t *context)> fn_emit_context_t;

/**
@internal
@typedef fn_emit_layout_t
@brief 
 */
typedef std::function<void(PangoLayout *)> fn_emit_layout_t;

/**
@internal
@typedef fn_emit_layout_a_t
@brief 
 */
typedef std::function<void(PangoLayout *, coordinate_t *)> fn_emit_layout_a_t;

/**
@internal
@typedef fn_emit_cr_layout_t
@brief 
 */
typedef std::function<void(cairo_t *cr, PangoLayout *)> fn_emit_cr_layout_t;

/**
@internal
@typedef fn_emit_overload_t
@brief 
 */
typedef std::variant<std::monostate, fn_emit_cr_t, fn_emit_cr_a_t,
                     fn_emit_context_t, fn_emit_layout_t, fn_emit_layout_a_t,
                     fn_emit_cr_layout_t>
    fn_emit_overload_t;

/**
 * @class visitor_interface_t
 * @brief holds information for the visitor interface. The function bind_dispatch
 * should be called after the immediately object is created.
 */
class visitor_interface_t {
public:
  fn_emit_overload_t fn={};
  std::size_t pipeline_order={};
  virtual void bind_dispatch(system_base_t *ptr) {}
};

/**
 * @class visitor_interfaces_t
 * @brief 
 */
class visitor_interfaces_base_t {
public:
  visitor_interfaces_base_t() {}
  virtual ~visitor_interfaces_base_t() {}
  std::unordered_map<std::type_index, visitor_interface_t *> accepted_interfaces =
      {};
};

/**
 * @class visitor_interfaces_t
 * @tparam ...
 * @brief class used within the unit declaration parameters
 * to note abstracts that are published.
 */
template <typename... Args>
class visitor_interfaces_t : public visitor_interfaces_base_t, public Args... {
public:
  visitor_interfaces_t() : Args(this)... {}
};

/**
 * @class system_base_t
 * @brief 
 */
class system_base_t {
public:
  system_base_t() {
    if (!visitor_dispatch_bound)
      ;
  }
  virtual ~system_base_t() {}
  void init_dispatch(void) {
  	auto p = dynamic_cast<visitor_interfaces_base_t *>(this);
    for (auto n : p->accepted_interfaces)
      n.second->bind_dispatch(this);
    visitor_dispatch_bound=true;
  }
  bool visitor_dispatch_bound = false;
};


  /** 
  @details the constructors of these abstract interface classes are 
  called by the visitor_interfaces_t param pack expansion.
  The interface position within the unordered_map accpeted_interfaces
  is reserved by setting the type id. Later the interface is resolved by iterating the map.
   This is done in two phases because at this point within the logic, the base class 
   which can be used in a polymorphic way is not available.
   The method bind_dispatch is called to tie the interface (member function) pointer
  to the instance of the implemented method. The system_base_t
  is used for this. There is a curisiousty that gdb does not recognize the fn
  holds a value, as it does.*/
  
/**
 * @class abstract_emit_context_t
 * @tparam std::size_t ORDER
 * @brief 
 */  
template <std::size_t ORDER> class abstract_emit_context_t : visitor_interface_t {
public:
  abstract_emit_context_t() {}

  abstract_emit_context_t(visitor_interfaces_base_t *ptr) {
  	pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(this))] =this;
    }
  
  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_context_t{std::bind(&abstract_emit_context_t::emit,
            dynamic_cast<abstract_emit_context_t *>(ptr), std::placeholders::_1)};
  }
    
  virtual ~abstract_emit_context_t() {}
  virtual void emit(display_context_t *context) = 0;

}; 

/**
 * @class abstract_emit_cr_t
 * @tparam std::size_t ORDER
 * @brief 
 */
template <std::size_t ORDER> class abstract_emit_cr_t : visitor_interface_t  {
public:
  abstract_emit_cr_t() {}
  abstract_emit_cr_t(visitor_interfaces_base_t *ptr) {
  	pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(this))] =this;
    }
  
  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_t{std::bind(&abstract_emit_cr_t::emit,
            dynamic_cast<abstract_emit_cr_t *>(ptr), std::placeholders::_1)};
  }
    
  virtual ~abstract_emit_cr_t() {}
  virtual void emit(cairo_t *cr) = 0;
};

/**
 * @class abstract_emit_cr_absolute_t
 * @tparam std::size_t ORDER
 * @brief 
 */
template <std::size_t ORDER> class abstract_emit_cr_absolute_t  : visitor_interface_t {
public:
  abstract_emit_cr_absolute_t() {}
  abstract_emit_cr_absolute_t(visitor_interfaces_base_t *ptr) {
  	pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(this))] =this;
    }
  
  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_t{std::bind(&abstract_emit_cr_absolute_t::emit_absolute,
            dynamic_cast<abstract_emit_cr_absolute_t *>(ptr), std::placeholders::_1)};
  }
    
  virtual ~abstract_emit_cr_absolute_t() {}
  virtual void emit_absolute(cairo_t *cr) = 0;
};

/**
 * @class abstract_emit_cr_relative_t
 * @tparam std::size_t ORDER
 * @brief 
 */
 template <std::size_t ORDER> class abstract_emit_cr_relative_t  : visitor_interface_t {
public:
  abstract_emit_cr_relative_t() {}
  abstract_emit_cr_relative_t(visitor_interfaces_base_t *ptr) {
  	pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(this))] =this;
    }
  
  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_t{std::bind(&abstract_emit_cr_relative_t::emit_relative,
            dynamic_cast<abstract_emit_cr_relative_t *>(ptr), std::placeholders::_1)};
  }  
  virtual ~abstract_emit_cr_relative_t() {}
  virtual void emit_relative(cairo_t *cr) = 0;
};

/**
 * @class abstract_emit_cr_a_t
 * @tparam std::size_t ORDER
 * @brief 
 */
template <std::size_t ORDER> class abstract_emit_cr_a_t  : visitor_interface_t {
public:
  abstract_emit_cr_a_t() {}
  abstract_emit_cr_a_t(visitor_interfaces_base_t *ptr) {
  	pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(this))] =this;
    }
  
  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_a_t{std::bind(&abstract_emit_cr_a_t::emit,
            dynamic_cast<abstract_emit_cr_a_t *>(ptr), std::placeholders::_1, std::placeholders::_2)};
  }    
  virtual ~abstract_emit_cr_a_t() {}
  virtual void emit(cairo_t *cr, coordinate_t *a) = 0;
};

/**
 * @class abstract_emit_layout_t
 * @tparam std::size_t ORDER
 * @brief 
 */
template <std::size_t ORDER> class abstract_emit_layout_t  : visitor_interface_t {
public:
  abstract_emit_layout_t() {}

  abstract_emit_layout_t(visitor_interfaces_base_t *ptr) {
  	pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(this))] =this;
    }
  
  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_layout_t{std::bind(&abstract_emit_layout_t::emit,
            dynamic_cast<abstract_emit_layout_t *>(ptr), std::placeholders::_1)};
  }  
  
  virtual ~abstract_emit_layout_t() {}
  virtual void emit(PangoLayout *layout) = 0;
};

/**
 * @class abstract_emit_layout_a_t
 * @tparam std::size_t ORDER
 * @brief 
 */
template <std::size_t ORDER> class abstract_emit_layout_a_t  : visitor_interface_t {
public:
  abstract_emit_layout_a_t() {}
  abstract_emit_layout_a_t(visitor_interfaces_base_t *ptr) {
  	pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(this))] =this;
    }
  
  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_layout_a_t{std::bind(&abstract_emit_layout_a_t::emit, 
                      dynamic_cast<abstract_emit_layout_a_t *>(ptr),
                      std::placeholders::_1, std::placeholders::_2)};
  } 
    
  virtual ~abstract_emit_layout_a_t(){};
  virtual void emit(PangoLayout *layout, coordinate_t *a) = 0;
};

/**
 * @class abstract_emit_cr_layout_t
 * @tparam std::size_t ORDER
 * @brief 
 */
template <std::size_t ORDER> class abstract_emit_cr_layout_t  : visitor_interface_t {
public:
  abstract_emit_cr_layout_t() {}
  abstract_emit_cr_layout_t(visitor_interfaces_base_t *ptr) {
  	pipeline_order = ORDER;
    ptr->accepted_interfaces[std::type_index(typeid(this))] =this;
    }
  
  void bind_dispatch(system_base_t *ptr) {
    fn = fn_emit_cr_layout_t{std::bind(&abstract_emit_cr_layout_t::emit, 
    				dynamic_cast<abstract_emit_cr_layout_t *>(ptr),
                      std::placeholders::_1, std::placeholders::_2)};
  } 
    
  virtual ~abstract_emit_cr_layout_t() {}
  virtual void emit(cairo_t *cr, PangoLayout *layout) = 0;
};

/**
 * @class visitor_base_t
 * @brief 
 */
class visitor_base_t {
public:
  visitor_base_t() {}
  virtual ~visitor_base_t() {}
};


/**
 * @class visitor_textual_render_t
 * @brief 
 */
class visitor_textual_render_t : virtual public visitor_base_t {
public:
  visitor_textual_render_t() {}
  virtual ~visitor_textual_render_t() {}
};

/**
 * @class visitor_image_block_t
 * @brief 
 */
class visitor_image_block_t : virtual public visitor_base_t {
public:
  visitor_image_block_t() {}
  virtual ~visitor_image_block_t() {}
};

} // namespace uxdevice


