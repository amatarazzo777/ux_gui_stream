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
 * @file mouse.cpp
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
#include <base/platform/mouse.h>

#include "os.h"
#include "window.h"
#include "mouse.h"

// clang-format on

/**
 * @internal
 * @fn mouse_device_xcb_t
 * @details handles the mouse device visits. The lambda functions below are
 * called based upon the visit for the variant. The variant is defined in
 * the class definition on the event_base_t template parameters. There must
 * be one entry here for each type mention within the parameters as well as
 * handling std::monostate which the base class inserts by default.
 * @returns mouse_device_xcb_t *
 */
uxdevice::mouse_device_xcb_t *uxdevice::mouse_device_xcb_t::get(void) {

  static const overload_visitors_t visit_map = overload_visitors_t{

      /**
       * @internal
       * @brief handles mouse move events.
       * @param motion_notify_xcb_t xcb
       */
      [&](motion_notify_xcb_t xcb) {
        x = xcb->event_x;
        y = xcb->event_y;
        alias = std::type_index{typeid(listen_mousemove_t)};
      },

      /**
       * @internal
       * @brief handles mouse button press. this also includes wheel for linux
       * which is 4 & 5 index.
       * @param button_press_xcb_t xcb
       */
      [&](button_press_xcb_t xcb) {
        // pluck relative items from the xcb event and place into
        // interface.
        x = xcb->event_x;
        y = xcb->event_y;

        /** @brief interpret these values as up and down wheel.*/
        if (xcb->detail == XCB_BUTTON_INDEX_4 ||
            xcb->detail == XCB_BUTTON_INDEX_5) {
          d = xcb->detail == XCB_BUTTON_INDEX_4 ? 1 : -1;
          alias = std::type_index{typeid(listen_wheel_t)};
        } else {
          d = xcb->detail;
          alias = std::type_index{typeid(listen_mousedown_t)};
        }
      },

      /**
       * @internal
       * @brief handles mouse button release
       * @param button_release_xcb_t xcb
       */
      [&](button_release_xcb_t xcb) {
        // ignore button 4 and 5 which are wheel events.
        if (xcb->detail != XCB_BUTTON_INDEX_4 &&
            xcb->detail != XCB_BUTTON_INDEX_5)
          alias = std::type_index{typeid(listen_mouseup_t)};
        else
          alias = std::type_index{typeid(mouse_device_xcb_t)};
      },
      [&](std::monostate) { alias = std::type_index{typeid(std::monostate)}; }};

  std::visit(visit_map, data);
  return this;
}
