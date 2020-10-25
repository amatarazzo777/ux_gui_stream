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
 * @file ux_os_linux_xcb_event.cpp
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
 * point might be acceptable.
 */
// clang-format off

#include <base/platform/object.h>
#include <base/platform/window.h>
#include <base/platform/keyboard.h>

#include "os.h"
#include "keyboard.h"
#include "window.h"

// clang-format on

/**
 * @fn  keyboard_device_event_t()
 * @brief constructor implementation
 *
 */
uxdevice::keyboard_device_xcb_t::keyboard_device_xcb_t()
    : keyboard_device_base_t() {

  syms = xcb_key_symbols_alloc(window_manager->connection);
  if (!syms) {
    std::stringstream sError;
    sError << "xcb_key_symbols_alloc "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }
}

/**
 * @fn  ~keyboard_device_event_t()
 * @brief
 *
 */
uxdevice::keyboard_device_xcb_t::~keyboard_device_xcb_t() {
  if (syms) {
    xcb_key_symbols_free(syms);
    syms = nullptr;
  }
}

/**
 * @fn uxdevice::event_t get(void)
 * @brief
 *
 * @return
 */
uxdevice::keyboard_device_xcb_t *uxdevice::keyboard_device_xcb_t::get(void) {
  auto visit_map = overload_visitors_t{

      /**
       * @internal
       * @brief handles keypress event
       * @param xcb_configure_notify_event_t *xcb
       */
      [&](key_press_xcb_t xcb) {
        // get key symbol
        sym = xcb_key_press_lookup_keysym(syms, xcb.get(), 0);

        // in range of keys?
        if (sym < 0x99) {

          // use xwindows to lookup the string
          XKeyEvent keyEvent;
          keyEvent.display = window_manager->xdisplay;
          keyEvent.keycode = xcb->detail;
          keyEvent.state = xcb->state;
          keyEvent.root = xcb->root;
          keyEvent.time = xcb->time;
          keyEvent.window = xcb->event;
          keyEvent.serial = xcb->sequence;

          // filter as a keypress event
          if (XLookupString(&keyEvent, c.data(), c.size(), nullptr, nullptr)) {
            alias = std::type_index{typeid(listen_keypress_t)};
          } else {
            // send a keydown event
            alias = std::type_index{typeid(listen_keydown_t)};
          }
        }
      },

      /**
       * @internal
       * @brief handles key release event
       * @param xcb_configure_notify_event_t *xcb
       */
      [&](key_release_xcb_t xcb) {
        sym = xcb_key_press_lookup_keysym(syms, xcb.get(), 0);
        alias = std::type_index{typeid(listen_keyup_t)};
      },
      [&](std::monostate) {
        sym = {};
        alias = std::type_index{typeid(std::monostate)};
      }};
  std::visit(visit_map, data);
  return this;
}
