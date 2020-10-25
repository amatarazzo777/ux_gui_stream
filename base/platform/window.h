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
@author Anthony Matarazzo
@file ux_os_window_manager_base.h
@date 10/15/20
@version 1.0
@brief
*/

namespace uxdevice {

/**
 * @typedef coordinate_list_t
 * @brief An std::list used to communicate coordinate for the window. varying
 * pairs may be given. two or four.
 */
typedef std::list<short int> coordinate_list_t;

/** @internal
 * @typedef
 * @brief system domain translator.
 */
using message_dispatch_t =
    std::unordered_map<std::size_t, std::function<void()>>;

typedef struct _WH {
  int w = 0;
  int h = 0;
  _WH(int _w, int _h) : w(_w), h(_h) {}
} __WH;

/**
 * @class window_manager_base_t
 * @brief class that all target OS window manager must inherit. The class
 * inherits publicly he window_manager_interface_t. That class provides
 * abstracts that must be implemented by each OS implementation. The
 * routines are small for the most part as the rest of the system invokes
 * them.
 *
 * This class, window_manager_base_t, provides the instantiation of a
 * message queue process and threaded approach to client interface. The base
 * provides interpretation of the parameters to form and fill in the base
 * object styles. These styles should be instantiated by the open_window()
 * function in the os implementation. That is it will received normalized
 * coordinates and structured information for building the window.
 *
 */
template <typename T>
class window_manager_base_t : virtual public system_error_t,
                              public window_manager_interface_t,
                              public message_queue_t<T> {
public:
  /**
   * @internal
   * @fn window_manager_interface_t
   * @brief Although the function is marked as internal, the interface is the
   * one that the caller sees within the window creation. The template
   * constructor only sets the properties. the set function is overloaded by
   * type which is how the parameters are decoded. Yet this is a base function
   * that is provided.*/
  template <typename... Args> window_manager_base_t(Args... args) {
    set(args...);
  }

  virtual ~window_manager_base_t() {}

  /// @brief move constructor
  window_manager_base_t(window_manager_base_t &&other) noexcept;

  /// @brief copy constructor
  window_manager_base_t(const window_manager_base_t &other);

  /// @brief copy assignment operator
  window_manager_base_t &operator=(const window_manager_base_t &other);

  /// @brief move assignment
  window_manager_base_t &operator=(window_manager_base_t &&other) noexcept;

  /// @brief mutex drawing function
  void draw_fn(const std::function<void(cairo_t *)> &fn);

  /// @brief mutex surface access
  void surface_fn(const std::function<void(cairo_surface_t *)> &fn);

  /// @brief sequence of the video frame flush
  void video_flush(void);

  /// @breif resize request from the window, queues them and then only processes
  /// the last one.
  void resize_surface(const int w, const int h);

  /** @brief function called at a point in time where the render within the
   * context is about to output a frame portion. This gives priorty to the user
   * resize so that blank areas do not appear on the screen for very long.*/
  void apply_surface_requests(void);

  /**
   * @internal
   * @fn set
   * @brief template abstract function requiring implementation in the inherited
   * class. The item types sent to the window constructor should be represented
   * in overloaded function form of {see below} type for the template.
   *
   *  template <> void set(const TYPE &setting) { ... }
   *
   *  note: the items within the instance should be held with the necessary
   * mutex.data_mutex is a generalized one. Other objects may need different
   * scope depending of their usage.
   */
  void set(const std::string &setting);

  void set(const coordinate_list_t &setting);

  void set(const painter_brush_t &setting);

  /**
   * @internal
   * @fn dispatch
   * @brief dispatch routine for the events.
   */
  template <typename DEVICE, typename MSG_CLASS>
  void dispatch(typename DEVICE::generalized_msg_t _msg) {
    auto o = DEVICE{MSG_CLASS{_msg}};
    auto evt = o.get();
    dispatch_event(evt);
  }

  template <typename T2> void dispatch_event(const T2 evt) {}

  /**
   * @internal
   * @fn set
   * @brief recursive variadic template to provide variable parameters. Simply
   * overload the type and provide interpretation within the logic.
   *
   */
  template <typename T2, typename... Args> void set(T2 setting, Args... args) {
    set(setting);
    set(args...);
  }



  /** @internal
   * @brief these variables are common across instances yet they are managed
   * by the inheriting object as part of base OS.
   */
  std::mutex data_mutex = {};
  short window_x = 0;
  short window_y = 0;
  unsigned short window_width = 0;
  unsigned short window_height = 0;
  unsigned short screen_width = 0;
  unsigned short screen_height = 0;
  std::string title;
  coordinate_list_t coordinate = {};

  bool window_open = false;

  std::bitset<64> window_style = {};

private:

  std::list<_WH> surface_requests_storage = {};

  typedef std::list<_WH>::iterator surface_requests_iter_t;
  std::mutex surface_requests_storage_mutex = {};

private:
  /** @internal
   * @brief these variables are common across instances yet they are managed
   * by the inheriting object as part of base OS. However the cairo_t
   * represents across platforms so it can be used like this. Minimizes and
   * simplifies code.
   */
  std::mutex surface_mutex = {};
  cairo_surface_t *surface = {};
  std::mutex cr_mutex = {};
  cairo_t *cr = {};
  std::mutex background_brush_mutex = {};
  painter_brush_t background_brush;
};

} // namespace uxdevice
