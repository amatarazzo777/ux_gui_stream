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
 * @file ux_device.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief rendering and platform services.
 * @details The file is the client API interface published the system. The main
 * class implementations are for surface_area_t. The rendering and message loops
 * for the base OS services are within this file. The start_processing function
 * is called by the window initialization constructors to create two threads,
 * one for the message queue and one for the rendering loop.
 */
#include <ux_device.h>

using namespace std;
using namespace uxdevice;

/**
 * @internal
 * @fn render_loop(void)
 * @details The routine is the main rendering thread. The thread runs when
 * necessary based upon a condition variable. Locks are placed on the surface
 * and rectangle list. The surface may change due to user resizing the gui
 * window so a spin flag is used to accommodate the functionality. That is
 * drawing cannot occur on the graphic device while the surface is being
 * resized.
 *
 * The surface_prime function checks to see if the surface exists. if so, the
 * two possible work flows are painting background rectangles that are caused by
 * the user resizing the window to a greater value. These values are inserted as
 * part of the paint event received from the base os message queue handler. This
 * function acquires locks on these small lists for the multi-threaded
 * necessity.
 *
 * The other work flow occurs when objects or their associated units change
 * values that must instantiate a redraw of the object. The hash_code() function
 * is used to detect changes.
 *
 * If no work exists, the surface_prime waits on the cvRenderWork condition
 * variable.
 */
void uxdevice::surface_area_t::render_loop(void) {
  while (bProcessing) {
    if (context.surface_prime())
      context.render();

    if (error_check()) {
      std::string errors = error_text();
      error_clear();
      cout << errors << std::flush;
    }
  }
}

/**
 * @internal
 * @fn dispatch_event
 * @param const event_t &evt
 * @brief the dispatch routine is invoked by the messageLoop. If default
 * handling is to be supplied, the method invokes the necessary operation.
 */
void uxdevice::surface_area_t::dispatch_event(const event_t &evt) {

  if (evt.type == std::type_index(typeid(listen_paint_t)))
    context.state_surface(evt.x, evt.y, evt.w, evt.h);
  else if (evt.type == std::type_index(typeid(listen_resize_t)))
    context.resize_surface(evt.w, evt.h);

  if (fnEvents)
    fnEvents(evt);
}
/**
 * @internal
 * @brief The entry point that processes messages from the operating system
 * application level services. Typically on Linux this is a coupling of xcb and
 * keysyms library for keyboard. Previous incarnations of technology such as
 * this typically used xserver. However, XCB is the newer form. Primarily
 * looking at the code of such programs as vlc, the routine simply places pixels
 * into the memory buffer. while on windows the direct x library is used in
 * combination with windows message queue processing.
 */
void uxdevice::surface_area_t::start_processing(void) {
  // setup the event dispatcher
  event_handler_t ev = std::bind(&uxdevice::surface_area_t::dispatch_event,
                                 this, std::placeholders::_1);
  context.cache_threshold = 2000;
  std::thread thrRenderer([=]() {
    bProcessing = true;
    render_loop();
  });

  thrRenderer.detach();

}

/**
 * @internal
 * @fn get_event_vector(eventType evtType)
 * @brief The function maps the event id to the appropriate vector. This is kept
 * statically here for retext_color_t management.
 * @param eventType evtType
 */
std::list<event_handler_t> &
uxdevice::surface_area_t::get_event_vector(std::type_index evtType) {
  static std::unordered_map<std::type_index, std::list<event_handler_t> &>
    eventTypeMap = {
      {std::type_index(typeid(listen_focus_t)), onfocus},
      {std::type_index(typeid(listen_blur_t)), onblur},
      {std::type_index(typeid(listen_resize_t)), onresize},
      {std::type_index(typeid(listen_keydown_t)), onkeydown},
      {std::type_index(typeid(listen_keyup_t)), onkeyup},
      {std::type_index(typeid(listen_keypress_t)), onkeypress},
      {std::type_index(typeid(listen_mouseenter_t)), onmouseenter},
      {std::type_index(typeid(listen_mouseleave_t)), onmouseleave},
      {std::type_index(typeid(listen_mousemove_t)), onmousemove},
      {std::type_index(typeid(listen_mousedown_t)), onmousedown},
      {std::type_index(typeid(listen_mouseup_t)), onmouseup},
      {std::type_index(typeid(listen_click_t)), onclick},
      {std::type_index(typeid(listen_dblclick_t)), ondblclick},
      {std::type_index(typeid(listen_contextmenu_t)), oncontextmenu},
      {std::type_index(typeid(listen_wheel_t)), onwheel}};
  auto it = eventTypeMap.find(evtType);
  return it->second;
}

#if 0
/**

\brief The function is invoked when an event occurs. Normally this occurs
from the platform device. However, this may be invoked by the soft
generation of events.

*/
void uxdevice::surface_area_t::dispatch(const event &e) {
  auto &v = getEventVector(e.evtType);
  for (auto &fn : v)
    fn(e);
}
#endif

/**
 * @overload
 * @fn surface_area_t() default window constructor.
 * @brief Opens a default window approximately 60% of window view area with the
 * program name according to cpp macros.
 */
uxdevice::surface_area_t::surface_area_t() {
  window_manager = std::make_shared<ux_os_xcb_linux_t>();
  context = std::make_shared<display_context_t>(window_manager);

  context->open_window(coordinate_list_t{}, DEFAULT_WINDOW_TITLE,
                       painter_brush_t{}, event_handler_t{});
}

/**
 * @overload
 * @fn surface_area_t() default window constructor.
 * @param const std::string &surface_area_title
 * @brief Opens a default window approximately 60% of window view area with the
 * title given. Default background, without an external window event handler.
 */
uxdevice::surface_area_t::surface_area_t(
  const std::string &surface_area_title) {
  window_manager = std::make_shared<ux_os_xcb_linux_t>();
  context = std::make_shared<display_context_t>(window_manager);

  context->open_window(coordinate_list_t{}, surface_area_title,
                       painter_brush_t{}, event_handler_t{});

  set_surface_defaults();
}

/**
 * @overload
 * @fn surface_area_t() default window constructor.
 * @param const coordinate_list_t &coordinate_t - coordinate and dimensions of
 * the window.
 * @brief Opens a default window approximately 60% of window view area with the
 * title given. Default background, without an external window event handler.
 */
uxdevice::surface_area_t::surface_area_t(const coordinate_list_t &coordinate) {
  window_manager = std::make_shared<ux_os_xcb_linux_t>();
  context = std::make_shared<display_context_t>(window_manager);

  context->open_window(coordinate, DEFAULT_WINDOW_TITLE, painter_brush_t{},
                       event_handler_t{});
}
/**
 * @overload
 * @fn surface_area_t() default window constructor.
 * @param const std::string &surface_area_title
 * @brief Opens a default window approximately 60% of window view area with the
 * title given. Default background, without an external window event handler.
 */
uxdevice::surface_area_t::surface_area_t(
  const event_handler_t &dispatch_events) {
  window_manager = std::make_shared<ux_os_xcb_linux_t>();
  context = std::make_shared<display_context_t>(window_manager);

  context->open_window(coordinate_list_t{}, DEFAULT_WINDOW_TITLE,
                       painter_brush_t{}, dispatch_events);
}

/**
 * @overload
 * @fn surface_area_t() default window constructor.
 * @param const std::string &surface_area_title
 * @brief Opens a default window approximately 60% of window view area with the
 * title given. Default background, without an external window event handler.
 */
uxdevice::surface_area_t::surface_area_t(
  const coordinate_list_t &coordinate, const std::string &surface_area_title) {
  window_manager = std::make_shared<ux_os_xcb_linux_t>();
  context = std::make_shared<display_context_t>(window_manager);

  context->open_window(coordinate, surface_area_title, painter_brush_t{},
                       event_handler_t{});
}

/**
 * @overload
 * @fn surface_area_t() default window constructor.
 * @param const coordinate_list_t &coordinate
 * @param const std::string &window_title
 * @param const painter_brush_t &background
 * @brief Opens a default window approximately 60% of window view area with the
 * title given. Default background, without an external window event handler.
 */
uxdevice::surface_area_t::surface_area_t(
  const coordinate_list_t &coordinate, const std::string &surface_area_title,
  const painter_brush_t &surface_background_brush) {

  window_manager = std::make_shared<ux_os_xcb_linux_t>();
  context = std::make_shared<display_context_t>(window_manager);

  context->open_window(coordinate, surface_area_title, surface_background_brush,
                       event_handler_t{});
}

/**
 * @overload
 * @fn surface_area_t() default window constructor.
 * @param const coordinate_list_t &coordinate
 * @param const std::string &window_title
 * @param const event_handler_t &dispatch_events
 * @param const painter_brush_t &surface_background_brush
 * @brief Opens a window the size and position of the coordinate provided. The
 * given title is used as the window title.  The background parameter serves as
 * the background surface brush, When the window is cleared, this is the
 * texture,color or gradient used. The window events such as focus, mouse,
 * resize and paint events may be listened to by the event dispatcher. The
 * event_handler_t type is used as the std::function storage type .
 */
uxdevice::surface_area_t::surface_area_t(
  const coordinate_list_t &coordinate, const std::string &window_title,
  const painter_brush_t &surface_background_brush,
  const event_handler_t &dispatch_events) {

  window_manager = std::make_shared<ux_os_xcb_linux_t>();
  context = std::make_shared<display_context_t>(window_manager);

  context->open_window(coordinate, window_title, surface_background_brush,
                       dispatch_events);
}

/**
 * @internal
 * @brief Destructor, closes a window on the target OS
 */
uxdevice::surface_area_t::~surface_area_t(void) { context->close_window(); }

/**
 * @internal
 * @brief sets the defaults for the context. font, colors, etc.
 */
void surface_area_t::set_surface_defaults(void) { SYSTEM_DEFAULTS }

/**
 * @brief API interface, just data is passed to objects. Objects are dynamically
 * allocated as classes derived from a unit base. Mutex is used one display list
 * to not get in the way of the rendering loop,
 */

/**
 * @fn clear(void)
 * @brief clears the display list and the context. However the brush is not
 * cleared.  The display and all objects are released.
 */
void uxdevice::surface_area_t::clear(void) {
  context.clear();
  display_list_clear();
}

/**
 * @fn notify_complete(void)
 * @brief An essential and very important function that releases the wait state
 * within the renderer. Without calling this function, no paint will occur
 * unless the timeout is met. The timeout is not added yet. Essentially this
 * increases the through put capabilities to top computer speed. This can show
 * system problems and deadlocks much easier and allow analysis of performance
 * issues or more informative cpu usage since the data perception is changed.
 */
void uxdevice::surface_area_t::notify_complete(void) {
  context.state_notify_complete();
}

/**
 * @internal
 * @fn maintain_index
 * @brief called by each of the display unit objects to index the item if a key
 * exists. A key can be given as a text_data_t or an integer. The [] operator is
 * used to access the data.
 */
void uxdevice::surface_area_t::maintain_index(
  const std::shared_ptr<display_unit_t> obj) {
  std::shared_ptr<key_storage_t> key_store =
    std::dynamic_pointer_cast<key_storage_t>(obj);

  if (!std::holds_alternative<std::monostate>(key_store->key))
    mapped_objects[key_store->key] = obj;
  return;
}

/**
 * @internal
 * @overload
 * @fn stream input
 * @param const std::string &s
 * @brief A stream interface routine that is declared using the
 * UX_DECLARE_STREAM_INTERFACE macro within the device published development
 * API. ux_device.hpp is where this is interface is declared.  The routine is
 * specialized because it creates a textual_rendering_t object that accepts the
 * textual data. Textual data is stored in a separate object. The
 * textual_rendering_t object encapsulates the pango cairo API functions which
 * is also added.
 */
surface_area_t &uxdevice::surface_area_t::stream_input(const std::string &s) {
  in(text_data_t{s}, textual_render_t{});
  return *this;
}

/**
 * @overload
 * @internal
 * @fn stream input
 * @param const std::shared_ptr<std::string> _val
 * @brief An overloaded stream interface implemetatione that is declared using
 * the UX_DECLARE_STREAM_INTERFACE macro inside the uxdevice::surface_area_t
 * class.
 * @details The routine is specialized because it creates a textual_rendering_t
 * object that accepts the textual data. Textual data is stored in a separate
 * object. The textual_rendering_t object encapsulates the pango cairo API
 * functions.
 */
surface_area_t &uxdevice::surface_area_t::stream_input(
  const std::shared_ptr<std::string> _val) {
  in(text_data_t{_val}.index(reinterpret_cast<std::size_t>(_val.get())),
     textual_render_t{});
  return *this;
}

/**
 * @overload
 * @internal
 * @fn stream input
 * @param const std::stringstream &_val
 * @brief An overloaded stream interface implementation that is declared using
 * the UX_DECLARE_STREAM_INTERFACE macro inside the uxdevice::surface_area_t
 * class. The macro only declares the interface prototypes. The implementation
 * for the specific input data type is below.
 * @details The routine is specialized because it creates a textual_rendering_t
 * object that accepts the textual data. Textual data is stored in a separate
 * object. The textual_rendering_t object encapsulates the pango cairo API
 * functions. A subtle effect of inserting a string stream verses a
 * shared_pointer to a stringstream is that a reference converts the item to a
 * string immediately, while inserting a pointer allows the caller to reuse the
 * string stream again. The contents of the shared_ptr<stringstream> are queried
 * at the time of render.
 */
surface_area_t &
uxdevice::surface_area_t::stream_input(const std::stringstream &_val) {
  in(text_data_t{_val.str()}, textual_render_t{});
  return *this;
}
surface_area_t &uxdevice::surface_area_t::stream_input(
  const std::shared_ptr<std::stringstream> _val) {
  in(text_data_t{_val}, textual_render_t{});
  return *this;
}

/**
 * @overload
 * @internal
 * @fn stream input
 * @param const std::string_view &s
 * @brief A stream interface routine that is declared using the
 * UX_DECLARE_STREAM_INTERFACE macro within the device published development
 * API. ux_device.hpp is where is interface is declared.
 *
 * The routine is specialized because it creates a textual_rendering_t object
 * that accepts the textual data. Textual data is stored in a separate object.
 * The textual_rendering_t object encapsulates the pango cairo API functions
 * which is also added.
 */
surface_area_t &
uxdevice::surface_area_t::stream_input(const std::string_view &s) {
  in(text_data_t{s}, textual_render_t{});

  return *this;
}

/**
 * @overload
 * @internal
 * @fn stream input
 * @param const std::shared_ptr<std::string_view> _val
 * @brief An overloaded stream interface implemetation that is declared inside
 * the uxdevice::surface_area_t class.
 * @details The routine is specialized because it creates a textual_rendering_t
 * object that accepts the textual data. Textual data is stored in a separate
 * object. The textual_rendering_t object encapsulates the pango cairo API
 * functions.
 */
surface_area_t &uxdevice::surface_area_t::stream_input(
  const std::shared_ptr<std::string_view> _val) {
  in(text_data_t{_val}.index(reinterpret_cast<std::size_t>(_val.get())),
     textual_render_t{});
  return *this;
}

/**
 * @fn save
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::save(void) {
  using namespace std::placeholders;
  display_list<function_object_t>(std::bind(cairo_save, _1));
  return *this;
}

/**
 * @fn restore
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::restore(void) {
  using namespace std::placeholders;
  display_list<function_object_t>(std::bind(cairo_restore, _1));
  return *this;
}

/**
 * @fn push
 * @param content_options_t c
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::push(content_options_t c) {
  using namespace std::placeholders;

  if (c == content_options_t::all)
    display_list<function_object_t>(std::bind(cairo_push_group, _1));
  else
    display_list<function_object_t>(std::bind(cairo_push_group_with_content, _1,
                                              static_cast<cairo_content_t>(c)));

  return *this;
}

/**
 * @fn pop
 * @param bool bToSource
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::pop(bool bToSource) {
  using namespace std::placeholders;

  if (bToSource)
    display_list<function_object_t>(std::bind(cairo_pop_group_to_source, _1));
  else
    display_list<function_object_t>(std::bind(cairo_pop_group, _1));

  return *this;
}

/**
 * @fn translate
 * @param painter_brush_t &b
 * @param double x
 * @param  double y
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::translate(double x, double y) {
  using namespace std::placeholders;
  display_list<function_object_t>(std::bind(cairo_translate, _1, x, y));
  return *this;
}

/**
 * @fn rotate
 * @param  double angle
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::rotate(double angle) {
  using namespace std::placeholders;
  display_list<function_object_t>(std::bind(cairo_rotate, _1, angle));
  return *this;
}

/**
 * @fn device_offset
 * @param  double x
 * @param  double y
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::device_offset(double x, double y) {
  context.device_offset(x, y);
  return *this;
}

/**
 * @fn device_scale
 * @param  double x
 * @param  double y
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::device_scale(double x, double y) {
  context.device_scale(x, y);
  return *this;
}

/**
 * @fn scale
 * @param  double x
 * @param  double y
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::scale(double x, double y) {
  using namespace std::placeholders;
  display_list<function_object_t>(std::bind(cairo_scale, _1, x, y));
  return *this;
}

/**
 * @fn transform
 * @param const matrix_t &m
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::transform(const matrix_t &m) {
  using namespace std::placeholders;
  display_list<function_object_t>(std::bind(cairo_transform, _1, &m._matrix));
  return *this;
}

/**
 * @fn matrix
 * @param const matrix_t &m
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::matrix(const matrix_t &m) {
  using namespace std::placeholders;
  display_list<function_object_t>(std::bind(cairo_set_matrix, _1, &m._matrix));
  return *this;
}

/**
 * @fn identity
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::identity(void) {
  using namespace std::placeholders;
  display_list<function_object_t>(std::bind(cairo_identity_matrix, _1));
  return *this;
}

/**
 * @fn device
 * @param double &x
 * @param  double &y
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::device(double &x, double &y) {
  using namespace std::placeholders;

  auto fn = [](cairo_t *cr, double &x, double &y) {
    double _x = x;
    double _y = y;
    cairo_user_to_device(cr, &_x, &_y);
    x = _x;
    y = _y;
  };

  display_list<function_object_t>(std::bind(fn, _1, x, y));

  return *this;
}

/**
 * @fn device_distance
 * @param double &x
 * @param  double &y
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::device_distance(double &x,
                                                          double &y) {
  using namespace std::placeholders;

  auto fn = [](cairo_t *cr, double &x, double &y) {
    double _x = x;
    double _y = y;
    cairo_user_to_device_distance(cr, &_x, &_y);
    x = _x;
    y = _y;
  };

  display_list<function_object_t>(std::bind(fn, _1, x, y));

  return *this;
}

/**
 * @fn user
 * @param double &x
 * @param  double &y
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::user(double &x, double &y) {
  using namespace std::placeholders;

  auto fn = [](cairo_t *cr, double &x, double &y) {
    double _x = x, _y = y;
    cairo_device_to_user(cr, &_x, &_y);
    x = _x;
    y = _y;
  };

  display_list<function_object_t>(std::bind(fn, _1, x, y));

  return *this;
}

/**
 * @fn user_distance
 * @param double &x
 * @param  double &y
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::user_distance(double &x, double &y) {
  using namespace std::placeholders;

  auto fn = [](cairo_t *cr, double &x, double &y) {
    double _x = x, _y = y;
    cairo_device_to_user_distance(cr, &_x, &_y);
    x = _x;
    y = _y;
  };

  display_list<function_object_t>(std::bind(fn, _1, x, y));

  return *this;
}

/**
 * @fn draw_caret
 * @param const int x
 * @param const int y
 * @param  const int h
 * @param painter_brush_t &b
 * @brief
 * @details
 */
void uxdevice::surface_area_t::draw_caret(const int x, const int y,
                                          const int h) {}

std::string _errorReport(std::string text_color_tFile, int ln,
                         std::string sfunc, std::string cond,
                         std::string ecode) {
  std::stringstream ss;
  ss << text_color_tFile << "(" << ln << ") " << sfunc << "  " << cond << ecode;
  return ss.str();
}
