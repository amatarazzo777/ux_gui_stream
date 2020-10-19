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
 * @file ux_device.hpp
 * @date 9/7/20
 * @version 1.0
 * @brief interface for the platform.
 */

/**
 * @namespace uxdevice
 * @brief The major invocation of API object usage is defined here.
 * surface_area_t and its constructors are here. The class provides the stream
 * operations for the system to provide an input mechanism. These are template
 * functions. Object composition and discrete initializations of the object is
 * accomplished here as well.
 *
 * Do not let clang format change the order of includes as order of dependency
 * is established in this serialized list that each of the modules includes. The
 * namespace uxdevice is used.
 *
 * The list of includes below is the major build order for the object class
 * definitions.
 */

// clang-format on
#include <ux_compile_options.h>
#include <ux_base.h>
#include <ux_system_error.h>
#include <ux_variant_visitor.h>
#include <ux_visitor_interface.h>
#include <ux_hash.h>
#include <ux_enums.h>
#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>
#include <ux_pipeline_memory.h>
#include <ux_display_visual.h>

#include <ux_display_context.h>
#include <ux_display_unit_base.h>
#include <ux_event_listeners.h>
#include <ux_os_window_manager_event_base.h>
#include <ux_os_window_manager_base.h>

#include <ux_coordinate.h>
#include <ux_textual_render.h>

// these files encompass the display unit objects which
// is published as the user object api.
#include <ux_text_units.h>
#include <ux_image_block_unit.h>
#include <ux_surface_area_units.h>
#include <ux_drawing_unit_primitives.h>


/// @brief os dependant
#include <ux_os_linux_xcb_event.h>
#include <ux_os_linux_xcb_window_manager.h>
// clang-format off

#pragma once

namespace uxdevice {

class event;

class bounds {
public:
  double x = 0, y = 0, w = 0, h = 0;
};

class point {
public:
  double x = 0, y = 0;
};

/**
 * @internal
 * @def UX_DECLARE_STREAM_INTERFACE
 * @brief the macro creates the stream interface for both constant references
 * and shared pointers as well as establishes the prototype for the insertion
 * function. The implementation is not standard and will need definition. This
 * is the route for formatting objects that accept numerical data and process to
 * human readable values. Modern implementations include the processing of size
 * information. Yet within the c++ implementation, the data structures that
 * report and hold information is elaborate.
 */

#define UX_DECLARE_STREAM_INTERFACE(CLASS_NAME)                                \
public:                                                                        \
  template <typename T> surface_area_t &operator<<(const CLASS_NAME &data) {   \
    stream_input(data);                                                        \
    return *this;                                                              \
  }                                                                            \
  template <typename T>                                                        \
  surface_area_t &operator<<(const std::shared_ptr<CLASS_NAME> data) {         \
    stream_input(data);                                                        \
    return *this;                                                              \
  }                                                                            \
                                                                               \
private:                                                                       \
  surface_area_t &stream_input(const CLASS_NAME &_val);                        \
  surface_area_t &stream_input(const std::shared_ptr<CLASS_NAME> _val);

/**
 * @class surface_area_t
 * @brief The main interface object of the system.
 */
class surface_area_t : public system_error_t {
public:
  surface_area_t();
  surface_area_t(const std::string &surface_area_title);

  surface_area_t(const event_handler_t &evtDispatcher);
  surface_area_t(const coordinate_list_t &coordinate);
  surface_area_t(const coordinate_list_t &coordinate,
                 const std::string &window_title,
                 const painter_brush_t &surface_background_brush,
                 const event_handler_t &dispatch_events);

  surface_area_t(const coordinate_list_t &coordinate,
                 const std::string &surface_area_title);
  surface_area_t(const coordinate_list_t &coordinate,
                 const std::string &surface_area_title,
                 const painter_brush_t &background);

  surface_area_t(const coordinate_list_t &coordinate,
                 const std::string &surface_area_title,
                 const event_handler_t &evtDispatcher,
                 const painter_brush_t &background);
  ~surface_area_t();

  // copy constructor
  surface_area_t(const surface_area_t &other)
    : system_error_t(other), context(other.context), fnEvents(other.fnEvents),
      display_list_storage(other.display_list_storage) {
    if (other.bProcessing)
      bProcessing = true;
  }

  // move constructor
  surface_area_t(surface_area_t &&other) noexcept
    : system_error_t(other), context(other.context), fnEvents(other.fnEvents),
      display_list_storage(other.display_list_storage) {
    if (other.bProcessing)
      bProcessing = true;
  }

  // copy assignment operator
  surface_area_t &operator=(const surface_area_t &other) {
    system_error_t::operator=(other);
    context = other.context;
    fnEvents = other.fnEvents;
    display_list_storage = other.display_list_storage;
    if (other.bProcessing)
      bProcessing = true;
    return *this;
  }
  // move assignment operator
  surface_area_t &operator=(surface_area_t &&other) noexcept {
    system_error_t::operator=(other);
    context = std::move(other.context);
    fnEvents = std::move(other.fnEvents);
    display_list_storage = std::move(other.display_list_storage);
    if (other.bProcessing)
      bProcessing = true;
    return *this;
  }

  /**
   * @fn operator<<
   * @tparam T
   * @brief The operator is a template function that also checks the base class
   * inheritance for the type of object as well as its exposed methods. The base
   * class inheritance signifies operations that occur specific to the object
   * type. The constexpr if states decide this at compile time which creates a
   * routine specific for the class and its described characteristics.
   */
  template <typename T> surface_area_t &operator<<(const T &data) {
    // event listeners are intercepted here.
    if constexpr (std::is_base_of<listener_t<T>, T>::value) {

    } else if constexpr (std::is_base_of<display_unit_t, T>::value) {
      std::shared_ptr<T> obj = display_list<T>(data);

      if constexpr (std::is_base_of<typed_index_t<T>, T>::value)
        maintain_index(std::dynamic_pointer_cast<display_unit_t>(obj));

      // the abstract interface function must be bound to an implementation.
      // the init_dispatch function accomplishes this.
      if constexpr (std::is_base_of<visitor_base_t, T>::value) {
        obj->init_dispatch();
        context->pipeline_memory_store<T>(obj);
      }

      // if the item is a drawing output object, inform the context of it.
      if constexpr (std::is_base_of<display_visual_t, T>::value)
        context->add_visual(obj);

      // otherwise the input is another type. Try
      // the default string stream.
    } else {
      std::ostringstream s;
      s << data;
      std::string sData = s.str();
      stream_input(sData);
    }

    return *this;
  }

  /**
   * @fn operator<<
   * @brief
   */
  template <typename T>
  surface_area_t &operator<<(const std::shared_ptr<T> obj) {

    // if the item is an event listener it is placed into a separate area.
    if constexpr (std::is_base_of<listener_t<T>, T>::value) {

      // display units are handled distinctly
    } else if constexpr (std::is_base_of<display_unit_t, T>::value) {
      display_list<T>(obj);
      if constexpr (std::is_base_of<typed_index_t<T>, T>::value)
        maintain_index(std::dynamic_pointer_cast<display_unit_t>(obj));

      // all objects that are tracked as visitors inherit from this base
      // the display context holds the entire list for all types of visitors.
      // the abstract interface function must be bound to an implementation.
      // the init_dispatch function accomplishes this.
      if constexpr (std::is_base_of<visitor_base_t, T>::value) {
        obj->init_dispatch();
        context->pipeline_memory_store<T>(obj);
      }

      // if the item is a drawing output object, inform the context of it.
      if constexpr (std::is_base_of<display_visual_t, T>::value)
        context->add_visual(obj);

    } else {
      stream_input(obj);
    }

    return *this;
  }

  /**
   * Declare interface only.  uxdevice.cpp contains implementation. These are
   * the stream interface with a function prototype for the invoke(). The
   * uxdevice.cpp file contains the implementation.  surface_area_t
   * &uxdevice::surface_area_t::stream_input( const CLASS_NAME _val) is the
   * prototype developed.
   */

  UX_DECLARE_STREAM_INTERFACE(std::string)
  UX_DECLARE_STREAM_INTERFACE(std::stringstream)
  UX_DECLARE_STREAM_INTERFACE(std::string_view)

  /** declares the interface and implementation for these objects when these are
   * invoked, the pipeline_memory class is also updated. When rendering objects
   * are created, text, image or other, these these shared pointers are used as
   * a reference local member initialized at invoke() public member. The
   * parameters and options are validated as well. */

  /**
   * @fn in
   * @tparam T - object to insert using the stream operator.
   * @tparam Args - list of them, param pack expansion calls recursively to
   * operator.
   * @brief An alternative input function to stream syntax.
   *  e.g. vis.in(text_font_t{"Arial 20px"}, coordindate_t{0,0}, "Hello");
   */
public:
  //template <typename T> void in(const T &&obj) { operator<<(obj); }
  template <typename T> void in(const T &obj) { operator<<(obj); }

  template <typename T, typename... Args>
  void in(const T &obj, const Args &... args) {
    operator<<(obj);
    in(args...);
  }

public:
  /**
   @fn
   @tparam T -
   @brief

   */
  template <typename T> T &operator[](const T &o) {
    std::shared_ptr<T> ptr = {};
    auto n = mapped_objects.find(o.key);
    if (n != mapped_objects.end()) {
      ptr = std::dynamic_pointer_cast<T>(n->second);
      ptr->changed();
    }
    return *ptr;
  }
  /**
   * @fn display_unit_t operator []&(const std::string&)
   * @brief
   *
   * @param _val
   * @return
   */
  display_unit_t &operator[](const std::string &_val) noexcept {
    auto n = mapped_objects.find(indirect_index_storage_t{_val});
    if (n != mapped_objects.end())
      n->second->changed();
    return *n->second;
  }
  /**
   * @fn T get&(const std::string&)
   * @brief
   *
   * @tparam T
   * @param key
   * @return
   */
  template <typename T> T &get(const std::string &key) {
    auto n = mapped_objects.find(indirect_index_storage_t{key});
    if (n != mapped_objects.end())
      n->second->changed();
    return *std::dynamic_pointer_cast<T>(n->second);
  }

  /**
   * @fn std::string operator []&(std::shared_ptr<std::string>)
   * @brief return display unit associated, update
   *
   * @param _val
   * @return
   */
  std::string &operator[](std::shared_ptr<std::string> _val) noexcept {
    auto n = mapped_objects.find(reinterpret_cast<std::size_t>(_val.get()));
    if (n != mapped_objects.end())
      n->second->changed();
    return *_val;
  }

  /**
   * @fn display_unit_t group&(const std::string&)
   * @brief
   *
   * @param sgroupname
   * @return
   */
  display_unit_t &group(const std::string &sgroupname) {
    auto n = mapped_objects.find(sgroupname);
    return *n->second;
  }

  bool processing(void) { return bProcessing; };
  surface_area_t &group(std::string &_name);

  surface_area_t &device_offset(double x, double y);
  surface_area_t &device_scale(double x, double y);
  void clear(void);
  void notify_complete(void);

  surface_area_t &save(void);
  surface_area_t &restore(void);

  surface_area_t &push(content_options_t _content = content_options_t::all);
  surface_area_t &pop(bool bToSource = false);

  surface_area_t &translate(double x, double y);
  surface_area_t &rotate(double angle);
  surface_area_t &scale(double x, double y);
  surface_area_t &transform(const matrix_t &mat);
  surface_area_t &matrix(const matrix_t &mat);
  surface_area_t &identity(void);
  surface_area_t &device(double &x, double &y);
  surface_area_t &device_distance(double &x, double &y);
  surface_area_t &user(double &x, double &y);
  surface_area_t &user_distance(double &x, double &y);
  point location(void);

  bounds stroke(void);
  bool in_stroke(double x, double y);
  bool in_fill(double x, double y);

  bounds clip(void);
  void clip(bool bPreserve = false);
  bool in_clip(double x, double y);

private:
  void start_processing(void);
  void draw_caret(const int x, const int y, const int h);

  void render_loop(void);
  void dispatch_event(const event_t &e);

  void set_surface_defaults(void);
  void maintain_index(const std::shared_ptr<display_unit_t> obj);

private:
  std::shared_ptr<window_manager_base_t> window_manager = {};
  std::shared_ptr<display_context_t> context = {};
  std::atomic<bool> bProcessing = false;

  event_handler_t fnEvents = nullptr;

  /**
   * @typedef display_unit_list_t
   * @brief type that holds the list that is input directly from the user. The
   * sequence is as it is input.
   */
  typedef std::list<std::shared_ptr<display_unit_t>> display_unit_list_t;

  /**
   * @var  display_list_storage
   * @brief holds the object units information as a display unit base. The base
   * may be casted to other object types using the std::dynamic_pointer_cast
   * function template. However, interface objects that use the multithreading
   * mutex types should be used. see display_list<T>
   *
   */
  display_unit_list_t display_list_storage = {};
  display_unit_list_t::iterator itDL_Processed = display_list_storage.begin();

  // interface between client and API rendering threads.
  std::mutex display_list_mutex = {};

  /**
   * @fn display_list
   * @brief template function to insert into the display list and perform
   * initialization based upon the type. The c++ constexpr conditional compiling
   * functionality is used to trim the run time and code size. One accepts a
   * constant double reference object in a parampack/
   */
  template <class T, typename... Args>
  std::shared_ptr<T> display_list(const Args &... args) {
    return display_list<T>(std::make_shared<T>(args...));
  }
  template <class T, typename... Args>

  std::shared_ptr<T> display_list(const Args &&... args) {
    return display_list<T>(std::make_shared<T>(args...));
  }
  /**
   * @fn display_list
   * @brief
   */
  template <class T>
  std::shared_ptr<T> display_list(const std::shared_ptr<T> ptr) {
    std::lock_guard<std::mutex> lock(display_list_mutex);
    display_list_storage.emplace_back(ptr);
    return ptr;
  }

  /**
   * @fn display_list_clear
   * @brief
   */
  void display_list_clear(void) {
    std::lock_guard<std::mutex> lock(display_list_mutex);
    display_list_storage.clear();
  }

  std::unordered_map<indirect_index_storage_t, std::shared_ptr<display_unit_t>>
    mapped_objects = {};

  std::list<event_handler_t> onfocus = {};
  std::list<event_handler_t> onblur = {};
  std::list<event_handler_t> onresize = {};
  std::list<event_handler_t> onkeydown = {};
  std::list<event_handler_t> onkeyup = {};
  std::list<event_handler_t> onkeypress = {};
  std::list<event_handler_t> onmouseenter = {};
  std::list<event_handler_t> onmouseleave = {};
  std::list<event_handler_t> onmousemove = {};
  std::list<event_handler_t> onmousedown = {};
  std::list<event_handler_t> onmouseup = {};
  std::list<event_handler_t> onclick = {};
  std::list<event_handler_t> ondblclick = {};
  std::list<event_handler_t> oncontextmenu = {};
  std::list<event_handler_t> onwheel = {};

  std::list<event_handler_t> &get_event_vector(std::type_index evt_type);
};

} // namespace uxdevice
