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
 * @file service.cpp
 * @date Oct 12, 2020
 * @version 1.0
 *
 * @details Implementation of events and their interpretation to the ux model
 * which is more colorful in descriptive names and less terse in nature. These
 * transpose from data xcb structures to objects as API. This is accomplished
 * using visitors for input devices. Their data is shipped from the message
 * intercept routine inside the object to a visitor routine. The class
 * implements a template get function and provides a discrete interface for each
 * supported type. The ability to add interpretations or gestures from this
 * point might be acceptable fro device specific readings. However other soft
 * events are generated for the application api such as begin drag, etc.
 */
// clang-format off

#include <base/platform/object.h>
#include <base/platform/window.h>
#include <base/platform/service.h>

#include "os.h"
#include "window.h"
#include "service.h"

// clang-format on

/**
 * @internal
 * @var keyboard_device_event_t::visit_map
 * @brief processing for different message from os, key up, down, and
 * virtuals.
 */
/// @brief move constructor
uxdevice::window_service_xcb_t::window_service_xcb_t(
    window_service_xcb_t &&other) noexcept
    : window_service_base_t(other), window_manager(other.window_manager) {}

/// @brief copy constructor
uxdevice::window_service_xcb_t::window_service_xcb_t(
    const window_service_xcb_t &other)
    : window_service_base_t(other), window_manager(other.window_manager) {}

/// @brief copy assignment operator
window_service_xcb_t &
uxdevice::window_service_xcb_t::operator=(const window_service_xcb_t &other) {
  window_service_base_t::operator=(other);
  window_manager = other.window_manager;
  return *this;
}

/// @brief move assignment
window_service_xcb_t &uxdevice::window_service_xcb_t::operator=(
    window_service_xcb_t &&other) noexcept {
  window_service_base_t::operator=(other);
  window_manager = other.window_manager;
  return *this;
}

/**
 * @var window_service_event_t::visit_map
 * @brief window services such as resize, move, and expose.
 */
uxdevice::window_service_xcb_t *uxdevice::window_service_xcb_t::get(void) {
  static const overload_visitors_t visit_map = overload_visitors_t{

      /**
       * @internal
       * @brief handles expose events which are translated into paint
       * @param surface_expose_xcb_t xcb
       */
      [&](surface_expose_xcb_t xcb) {
        x = xcb->x;
        y = xcb->y;
        w = xcb->width;
        h = xcb->height;
        alias = std::type_index{typeid(listen_paint_t)};
      },

      /**
       * @internal
       * @brief handles resize event
       * @param configure_notify_xcb_t xcb
       */
      [&](configure_notify_xcb_t xcb) {
        if (xcb->window == window_manager->window &&
            ((short)xcb->width != window_manager->window_width ||
             (short)xcb->height != window_manager->window_height)) {
          w = xcb->width;
          h = xcb->height;

          bvideo_output = true;
          alias = std::type_index{typeid(listen_resize_t)};
        }
      },

      /**
       * @internal
       * @brief handles client message such as close window.
       * @param client_message_xcb_t xcb
       */
      [&](client_message_xcb_t xcb) {
        // filter subset for this... original from stack over flow
        if (xcb->data.data32[0] == window_manager->reply2->atom) {
          alias = std::type_index{typeid(listen_close_window_t)};
        }
      },
      [&](std::monostate) { alias = std::type_index{typeid(std::monostate)}; }};

  std::visit(visit_map, data);

  return this;
}
