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
 * which is more colorful descriptive and less terse in nature. These transpose
 * from data xcb structures to objects as API. This is accomplished using
 * visitors for input devices. Their data is shipped from the message intercept
 * routine inside the object to a visitor routine. The class implements a
 * template get function and provides a discrete interface for each supported
 * type. The ability to add interpretations or gestures from this point might be
 * acceptable.
 */
#include <ux_compile_options.h>
#include <ux_base.h>
#include <ux_system_error.h>
#include <ux_hash.h>
#include <ux_variant_visitor.h>
#include <ux_enums.h>
#include <ux_visitor_interface.h>
#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>

#include <ux_os_window_manager_event_base.h>
#include <ux_os_window_manager_base.h>
#include <ux_os_linux_xcb_event.h>

/**
 * @fn  keyboard_device_event_t()
 * @brief
 *
 */
uxdevice::keyboard_device_event_t::keyboard_device_event_t(std::type_index ti)
  : keyboard_device_event_base_t(ti) {
  ///@brief The alias is a signature that provides meaning to the information
  /// that is specific to how the system will interpret it. These are names
  /// within the API such as listen_mousemove_t. The data is kept as unit along
  /// with the alias.
  syms = xcb_key_symbols_alloc(window_manager->connection);
  if (!syms) {
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }
}

/**
 * @fn  ~keyboard_device_event_t()
 * @brief
 *
 */
uxdevice::keyboard_device_event_t::~keyboard_device_event_t() {
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
uxdevice::event_t uxdevice::keyboard_device_event_t::get(void) {
  static const overload_visitors_t visit_map = {

    /**
     * @internal
     * @brief handles keypress event
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [&](xcb_key_press_event_t *xcb) {
      // get key symbol
      sym = xcb_key_press_lookup_keysym(syms, xcb, 0);

      // in range of keys?
      if (sym < 0x99) {

        // use xwindows to lookup the string
        XKeyEvent keyEvent;
        keyEvent.display = context->xdisplay;
        keyEvent.keycode = xcb->detail;
        keyEvent.state = xcb->state;
        keyEvent.root = xcb->root;
        keyEvent.time = xcb->time;
        keyEvent.window = xcb->event;
        keyEvent.serial = xcb->sequence;

        // filter as a keypress event
        if (XLookupString(&keyEvent, c.data(), c.size(), nullptr, nullptr))
          return event_t{std::type_index{typeid(listen_keypress_t)}, this};

      } else {

        // send a keydown event
        return event_t{std::type_index{typeid(listen_keydown_t)}, this};
      }
    },

    /**
     * @internal
     * @brief handles key release event
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [&](xcb_key_release_event_t *e) {
      sym = xcb_key_press_lookup_keysym(context.syms, e, 0);
      return event_t { std::type_index{typeid(listen_keyup_t)}, this }
    },

    /**
     * @internal
     * @brief handles undefined message
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [&](std::monostate) {
      return event_t{std::type_index{typeid(event_t)}, this};
    }};

  return std::visit(visit_map, data);
}
/**
 * @internal
 * @var keyboard_device_event_t::visit_map
 * @brief processing for different message from os, key up, down, and virtuals.
 */

/**
 * @internal
 * @var mouse_device_event_t
 * @details handles the mouse device visits. The lambda functions below are
 * called based upon the visit for the variant. The variant is defined in the
 * class definition on the event_base_t template parameters. There must be one
 * entry here for each type mention within the parameters as well as handling
 * std::monostate which the base class inserts by default.
 */
const uxdevice::mouse_device_event_t::overload_visitors_t event_t get(void) {

  static const overload_visitors_t visit_map = overload_visitors_t{

    /**
     * @internal
     * @brief handles mouse move events.
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [](xcb_motion_notify_event_t *xcb) {
      event_t ret = {};
      object->x = xcb->event_x;
      object->y = xcb->event_y;

      ret = event_t{std::type_index{typeid(listen_mousemove_t)}, this};

      return ret;
    },

    /**
     * @internal
     * @brief handles mouse button press. this also includes wheel for linux
     * which is 4 & 5 index.
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [](xcb_button_press_event_t *xcb) {
      event_t ret = {};

      // pluck relative items from the xcb event and place into
      // interface.
      object->x = xcb->event_x;
      object->y = xcb->event_y;

      /** @brief interpret these values as up and down wheel.*/
      if (xcb->detail == XCB_BUTTON_INDEX_4 ||
          xcb->detail == XCB_BUTTON_INDEX_5) {
        object->d = xcb->detail == XCB_BUTTON_INDEX_4 ? 1 : -1;

        ret = event_t{std::type_index{typeid(listen_wheel_t)}, object};

      } else {

        object->d = xcb->detail;
        ret = event_t{std::type_index{typeid(listen_mousedown_t)}, this};
      }

      return ret;
    },

    /**
     * @internal
     * @brief handles mouse button release
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [](xcb_button_release_event_t *e) {
      // ignore button 4 and 5 which are wheel events.
      if (e->detail != XCB_BUTTON_INDEX_4 && e->detail != XCB_BUTTON_INDEX_5)
        return event_t{std::type_index{typeid(listen_mouseup_t)}, this};
    },

    /**
     * @internal
     * @brief handles undefined event
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [](std::monostate) {
      return event_t{std::type_index{typeid(event_t)}, std::monostate};
    }};
  return std::visit(visit_map, data);
}

/**
 * @var window_service_event_t::visit_map
 * @brief window services such as resize, move, and expose.
 */
event_t uxdevice::window_service_event_t::get(void) {
  static const overload_visitors_t visit_map = overload_visitors_t{

    /**
     * @internal
     * @brief handles expose events which are translated into paint
     * @param window_service_event_t *object
     * @param xcb_expose_event_t *xcb
     */
    [&](xcb_expose_event_t *xcb) {
      x = xcb->x;
      y = xcb->y;
      w = xcb->width;
      h = xcb->height;

      return event_t{std::type_index{typeid(listen_paint_t)}, this};
    },

    /**
     * @internal
     * @brief handles resize event
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [&](xcb_configure_notify_event_t *xcb) {
      if (xcb->window == window_manager->window &&
          ((short)xcb->width != window_manager->window_width ||
           (short)xcb->height != window_manager->window_height)) {
        w = xcb->width;
        h = xcb->height;

        bvideo_output = true;
        return event_t{std::type_index{typeid(listen_resize_t)}, this};
      }
    },

    /**
     * @internal
     * @brief handles client message such as close window.
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [&](xcb_client_message_event_t *xcb) {
      // filter subset for this... original from stack over flow
      if (xcb->data.data32[0] == reply2->atom) {
        return event_t{std::type_index{typeid(listen_close_window_t)}, this};
      }
    },

    /**
     * @internal
     * @brief handles undefined message type
     * @param window_service_event_t *object
     * @param xcb_configure_notify_event_t *xcb
     */
    [&](std::monostate) {
      return event_t{std::type_index{typeid(event_t)}, std::monostate};
    }};

  return std::visit(visit_map, data);
}
