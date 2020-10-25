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
 * @file device.cpp
 * @date 9/7/20
 * @version 1.0
 * @brief rendering and platform services.
 * @details The file is the client API interface published the system. The main
 * class implementations are for surface_area_t. The rendering and message loops
 * for the base OS services are within this file. The start_processing function
 * is called by the window initialization constructors to create two threads,
 * one for the message queue and one for the rendering loop.
 */

// clang-format off
#include <api/device.h>
#include <api/options.h>
#include <base/platform/object.h>
#include <base/platform/window.h>
#include <base/unit_object.h>

#include <base/render_base.h>
#include <api/units.h>

// clang-format on

using namespace std;
using namespace uxdevice;

// copy constructor
uxdevice::surface_area_t::surface_area_t(const surface_area_t &other)
    : system_error_t(other), fnEvents(other.fnEvents) {
  if (other.bProcessing)
    bProcessing = true;
}

// move constructor
uxdevice::surface_area_t::surface_area_t(surface_area_t &&other) noexcept
    : system_error_t(other), fnEvents(other.fnEvents) {
  if (other.bProcessing)
    bProcessing = true;
}

// copy assignment operator
uxdevice::surface_area_t &
uxdevice::surface_area_t::operator=(const surface_area_t &other) {
  system_error_t::operator=(other);

  fnEvents = other.fnEvents;

  if (other.bProcessing)
    bProcessing = true;
  return *this;
}

// move assignment operator
uxdevice::surface_area_t &operator=(surface_area_t &&other) noexcept {
  system_error_t::operator=(other);

  fnEvents = std::move(other.fnEvents);

  if (other.bProcessing)
    bProcessing = true;
  return *this;
}

/**
 * @internal
 * @brief Destructor, closes a window on the target OS
 */
uxdevice::surface_area_t::~surface_area_t(void) {}

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
void uxdevice::surface_area_t::clear(void) { gui_shared_library->fn_clear(); }

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
  gui_shared_library->fn_notify_complete();
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
  static const interface_alias_t interface_alias_c_ptr = interface_alias_t{
      "char_c_ptr", VERSION, "e864239c-2258-4fb4-8227-4b08de760300"};

  gui_shared_library->fn_input_resource(interface_alias_c_ptr, s.data(),
                                        s.size());

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

  static const interface_alias_t interface_alias_shared_c_ptr =
      interface_alias_t{"shared_char_c_ptr", VERSION,
                        "c57e1ebc-0e9d-4bc3-8034-78942c6e4358"};

  gui_shared_library->fn_input_shared_resource(interface_alias_shared_c_ptr,
                                               _val.data(), val.size());

  return *this;
}

/**
 * @fn save
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::save(void) {
  gui_shared_library->fn_save();
  return *this;
}

/**
 * @fn restore
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::restore(void) {
  gui_shared_library->fn_restore();
  return *this;
}

/**
 * @fn push
 * @param content_options_t c
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::push(content_options_t c) {
  gui_shared_library->fn_push(c);

  return *this;
}

/**
 * @fn pop
 * @param bool bToSource
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::pop(bool bToSource) {
  gui_shared_library->fn_pop(bToSource);

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
  gui_shared_library->fn_translate(x, y);
  return *this;
}

/**
 * @fn rotate
 * @param  double angle
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::rotate(double angle) {
  gui_shared_library->fn_rotate(angle);
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
  gui_shared_library->fn_device_offset(x, y);
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
  gui_shared_library->fn_device_scale(x, y);
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
  gui_shared_library->fn_scale(x, y);
  return *this;
}

/**
 * @fn transform
 * @param const matrix_t &m
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::transform(const matrix_t &m) {
  gui_shared_library->fn_transform(m);
  return *this;
}

/**
 * @fn matrix
 * @param const matrix_t &m
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::matrix(const matrix_t &m) {
  gui_shared_library->fn_matrix(m);
  return *this;
}

/**
 * @fn identity
 * @brief
 * @details
 */
surface_area_t &uxdevice::surface_area_t::identity(void) {
  using namespace std::placeholders;
  gui_shared_library->fn_identity();
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
  gui_shared_library->fn_device(x, y);
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
  gui_shared_library->fn_device_distance(x, y);

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
  gui_shared_library->fn_user(x, y);
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
  gui_shared_library->fn_user_distance(x, y);

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
  gui_shared_library->fn_device(x, y);
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
  gui_shared_library->fn_device_distance(x, y);

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
  gui_shared_library->fn_user(x, y);

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
  gui_shared_library->fn_user_distance(x, y);
  return *this;
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
  gui_shared_library->fn_notify_complete();
}
