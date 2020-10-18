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
 * @file ux_os_xcb_linux.h
 * @date Oct 12, 2020
 * @version 1.0
 * @details The file implements the os interface which provides the
 * communication to the os for basic visualization services such as window
 * manager functions. THe message queue processor is defined and implemented
 * here..
 */

#include "ux_os_xcb_linux.h"

/**
 * @internal
 * @fn open_window
 * @param const coordinate_list_t &coord
 * @param const std::string &sWindowTitle
 * @param const painter_brush_t &background
 * @param const event_handler_t &dispatch_events
 * @brief opens a window on the target OS. used by all of the constructors.
 * parameters may be null or defaulted.
 */
void uxdevice::os_xcb_linux_t::open_window(
  const coordinate_list_t &coord, const std::string &sWindowTitle,
  const painter_brush_t &background, const event_handler_t &dispatch_events) {
  auto it = coord.begin();

  context.window_width = *it;
  it++;
  context.window_height = *it;
  context.brush = background;

  // this open provides interoperability between xcb and xwindows
  // this is used here because of the necessity of key mapping.
  context.xdisplay = XOpenDisplay(nullptr);
  if (!context.xdisplay) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* get the connection to the X server */
  context.connection = XGetXCBConnection(context.xdisplay);
  if (!context.xdisplay) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* Get the first screen */
  context.screen =
    xcb_setup_roots_iterator(xcb_get_setup(context.connection)).data;
  if (!context.screen) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  context.syms = xcb_key_symbols_alloc(context.connection);
  if (!context.syms) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* Create black (foreground) graphic context */
  context.window = context.screen->root;
  context.graphics = xcb_generate_id(context.connection);
  uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  uint32_t values[] = {context.screen->black_pixel, 0};
  xcb_create_gc(context.connection, context.graphics, context.window, mask,
                values);

  if (!context.graphics) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* Create a window */
  context.window = xcb_generate_id(context.connection);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  mask = XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK;
  mask = XCB_CW_BORDER_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_OVERRIDE_REDIRECT |
         XCB_CW_SAVE_UNDER | XCB_CW_EVENT_MASK;

  uint32_t vals[] = {
    context.screen->black_pixel, XCB_GRAVITY_NORTH_WEST, 0, 1,
    XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS |
      XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_BUTTON_MOTION | XCB_EVENT_MASK_BUTTON_PRESS |
      XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_STRUCTURE_NOTIFY};

  xcb_create_window(context.connection, XCB_COPY_FROM_PARENT, context.window,
                    context.screen->root, 0, 0,
                    static_cast<unsigned short>(context.window_width),
                    static_cast<unsigned short>(context.window_height), 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, context.screen->root_visual,
                    mask, vals);
  // set window title
  xcb_change_property(context.connection, XCB_PROP_MODE_REPLACE, context.window,
                      XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, sWindowTitle.size(),
                      sWindowTitle.data());

  if (!context.window) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* you init the connection and screen_nbr */
  xcb_depth_iterator_t depth_iter;

  depth_iter = xcb_screen_allowed_depths_iterator(context.screen);
  for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
    xcb_visualtype_iterator_t visual_iter;

    visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
    for (; visual_iter.rem; xcb_visualtype_next(&visual_iter)) {
      if (context.screen->root_visual == visual_iter.data->visual_id) {
        context.visualType = visual_iter.data;
        break;
      }
    }
  }

  // create xcb surface

  context.xcbSurface = cairo_xcb_surface_create(
    context.connection, context.window, context.visualType,
    context.window_width, context.window_height);
  if (!context.xcbSurface) {
    close_window();
    std::stringstream sError;
    sError << "ERR_CAIRO "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  // create cairo context
  context.cr = cairo_create(context.xcbSurface);
  if (!context.cr) {
    close_window();
    std::stringstream sError;
    sError << "ERR_CAIRO "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* Map the window on the screen and flush*/
  xcb_map_window(context.connection, context.window);
  xcb_flush(context.connection);
  context.window_open = true;

  cairo_surface_flush(context.xcbSurface);
  start_processing();

  return;
}

/**
 * @internal
 * @fn close_window
 * @brief closes a window on the target OS
 */
void uxdevice::os_xcb_linux_t::close_window(void) {

  if (context.xcbSurface) {
    cairo_surface_destroy(context.xcbSurface);
    context.xcbSurface = nullptr;
  }

  if (context.cr) {
    cairo_destroy(context.cr);
    context.cr = nullptr;
  }

  if (context.graphics) {
    xcb_free_gc(context.connection, context.graphics);
    context.graphics = 0;
  }

  if (context.syms) {
    xcb_key_symbols_free(context.syms);
    context.syms = nullptr;
  }

  if (context.window) {
    xcb_destroy_window(context.connection, context.window);
    context.window = 0;
  }

  if (context.xdisplay) {
    XCloseDisplay(context.xdisplay);
    context.xdisplay = nullptr;
  }

  context.window_open = false;
}

/**
 * @internal
 * @fn message_loop
 * @brief the routine handles the message processing for the specific operating
 * system. The function is called from processEvents.
 */
void uxdevice::os_xcb_linux_t::message_loop(void) {
  xcb_generic_event_t *xcbEvent;

  // is window open?
  while (bProcessing && !context.connection)
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
  if (!context.connection)
    return;

  // setup close window event
  xcb_intern_atom_cookie_t cookie =
    xcb_intern_atom(context.connection, 1, 12, "WM_PROTOCOLS");
  xcb_intern_atom_reply_t *reply =
    xcb_intern_atom_reply(context.connection, cookie, 0);

  xcb_intern_atom_cookie_t cookie2 =
    xcb_intern_atom(context.connection, 0, 16, "WM_DELETE_WINDOW");
  xcb_intern_atom_reply_t *reply2 =
    xcb_intern_atom_reply(context.connection, cookie2, 0);

  xcb_change_property(context.connection, XCB_PROP_MODE_REPLACE, context.window,
                      (*reply).atom, 4, 32, 1, &(*reply2).atom);

  // process Message queue
  std::list<xcb_generic_event_t *> xcbEvents;
  bool bvideo_output = false;
  while (bProcessing && (xcbEvent = xcb_wait_for_event(context.connection))) {
    xcbEvents.emplace_back(xcbEvent);
    bvideo_output = false;
    // qt5 does this, it queues all of the input messages at once.
    // this makes the processing of painting and reading input faster.
    while (bProcessing &&
           (xcbEvent = xcb_poll_for_queued_event(context.connection)))
      xcbEvents.emplace_back(xcbEvent);

    while (!xcbEvents.empty()) {
      xcbEvent = xcbEvents.front();
      switch (xcbEvent->response_type & ~0x80) {
      case XCB_MOTION_NOTIFY: {
        xcb_motion_notify_event_t *motion =
          (xcb_motion_notify_event_t *)xcbEvent;
        dispatch_event(event_t{
          std::type_index{typeid(listen_mousemove_t)},
          (short)motion->event_x,
          (short)motion->event_y,
        });
      } break;
      case XCB_BUTTON_PRESS: {
        xcb_button_press_event_t *bp = (xcb_button_press_event_t *)xcbEvent;
        if (bp->detail == XCB_BUTTON_INDEX_4 ||
            bp->detail == XCB_BUTTON_INDEX_5) {
          dispatch_event(
            event_t{std::type_index{typeid(listen_wheel_t)}, (short)bp->event_x,
                    (short)bp->event_y,
                    (short)(bp->detail == XCB_BUTTON_INDEX_4 ? 1 : -1)});

        } else {

          dispatch_event(event_t{std::type_index{typeid(listen_mousedown_t)},
                                 (short)bp->event_x, (short)bp->event_y,
                                 (short)bp->detail});
        }
      } break;
      case XCB_BUTTON_RELEASE: {
        xcb_button_release_event_t *br = (xcb_button_release_event_t *)xcbEvent;
        // ignore button 4 and 5 which are wheel events.
        if (br->detail != XCB_BUTTON_INDEX_4 &&
            br->detail != XCB_BUTTON_INDEX_5)
          dispatch_event(event_t{std::type_index{typeid(listen_mouseup_t)},
                                 (short)br->event_x, (short)br->event_y,
                                 (short)br->detail});
      } break;
      case XCB_KEY_PRESS: {
        xcb_key_press_event_t *kp = (xcb_key_press_event_t *)xcbEvent;
        xcb_keysym_t sym = xcb_key_press_lookup_keysym(context.syms, kp, 0);
        if (sym < 0x99) {
          XKeyEvent keyEvent;
          keyEvent.display = context.xdisplay;
          keyEvent.keycode = kp->detail;
          keyEvent.state = kp->state;
          keyEvent.root = kp->root;
          keyEvent.time = kp->time;
          keyEvent.window = kp->event;
          keyEvent.serial = kp->sequence;

          std::array<char, 316> buf{};
          if (XLookupString(&keyEvent, buf.data(), buf.size(), nullptr,
                            nullptr))
            dispatch_event(event_t{std::type_index{typeid(listen_keypress_t)},
                                   (char)buf[0]});
        } else {
          dispatch_event(
            event_t{std::type_index{typeid(listen_keydown_t)}, sym});
        }
      } break;
      case XCB_KEY_RELEASE: {
        xcb_key_release_event_t *kr = (xcb_key_release_event_t *)xcbEvent;
        xcb_keysym_t sym = xcb_key_press_lookup_keysym(context.syms, kr, 0);
        dispatch_event(event_t{std::type_index{typeid(listen_keyup_t)}});
      } break;
      case XCB_EXPOSE: {
        xcb_expose_event_t *eev = (xcb_expose_event_t *)xcbEvent;

        dispatch_event(event_t{std::type_index{typeid(listen_paint_t)},
                               (short)eev->x, (short)eev->y, (short)eev->width,
                               (short)eev->height});
        bvideo_output = true;
      } break;
      case XCB_CONFIGURE_NOTIFY: {
        const xcb_configure_notify_event_t *cfgEvent =
          (const xcb_configure_notify_event_t *)xcbEvent;

        if (cfgEvent->window == context.window &&
            ((short)cfgEvent->width != context.window_width ||
             (short)cfgEvent->height != context.window_height)) {
          dispatch_event(event_t{std::type_index{typeid(listen_resize_t)},
                                 (short)cfgEvent->width,
                                 (short)cfgEvent->height});
          bvideo_output = true;
        }
      } break;
      case XCB_CLIENT_MESSAGE: {
        if ((*(xcb_client_message_event_t *)xcbEvent).data.data32[0] ==
            (*reply2).atom) {
          bProcessing = false;
        }
      } break;
      }
      free(xcbEvent);
      xcbEvents.pop_front();
    }

    if (bvideo_output)
      context.state_notify_complete();
  }
}
