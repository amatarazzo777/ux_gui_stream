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
 * manager functions. The message queue processor is defined and implemented
 * here.
 */
#include <ux_base.h>
#include <ux_system_error.h>
#include <ux_visitor_interface.h>
#include <ux_hash.h>
#include <ux_enums.h>
#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>
#include <ux_display_unit_base.h>
#include <ux_event_listeners.h>
#include <ux_os_window_manager_event_base.h>
#include <ux_os_window_manager_base.h>
#include <ux_os_linux_xcb_event.h>
#include <ux_os_linux_xcb_window_manager.h>

/**
 * @fn uxdevice::os_xcb_linux_t operator =&(const os_xcb_linux_t&)
 * @brief copy assignment operator
 *
 * @param other
 * @return
 */
uxdevice::os_xcb_linux_t &
uxdevice::os_xcb_linux_t::operator=(const os_xcb_linux_t &other) {
  window_x = other.window_x;
  window_y = other.window_y;
  window_width = other.window_width;
  window_height = other.window_height;
  window_open = other.window_open;

  xdisplay = other.xdisplay;
  connection = other.connection;
  screen = other.screen;
  window = other.window;
  graphics = other.graphics;
  visual_type = other.visual_type;
  surface = cairo_surface_reference(other.surface);
  cr = cairo_reference(other.cr);
  return *this;
}

/**
 * @fn uxdevice::os_xcb_linux_t operator =&(os_xcb_linux_t&&)
 * @brief move assignment operator
 *
 * @param other
 * @return
 */
uxdevice::os_xcb_linux_t &
uxdevice::os_xcb_linux_t::operator=(os_xcb_linux_t &&other) noexcept {
  window_x = other.window_x;
  window_y = other.window_y;
  window_width = other.window_width;
  window_height = other.window_height;
  window_open = other.window_open;

  xdisplay = other.xdisplay;
  connection = other.connection;
  screen = other.screen;
  window = other.window;
  graphics = other.graphics;
  visual_type = other.visual_type;
  surface = other.surface;
  return *this;
}

/**
 *
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

  window_width = *it;
  it++;
  window_height = *it;

  // this open provides interoperability between xcb and xwindows
  // this is used here because of the necessity of key mapping.
  xdisplay = XOpenDisplay(nullptr);
  if (!xdisplay) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* get the connection to the X server */
  connection = XGetXCBConnection(xdisplay);
  if (!xdisplay) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* Get the first screen */
  screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
  if (!screen) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* Create black (foreground) graphic context */
  window = screen->root;
  graphics = xcb_generate_id(connection);
  uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  uint32_t values[] = {screen->black_pixel, 0};
  xcb_create_gc(connection, graphics, window, mask, values);

  if (!graphics) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* Create a window */
  window = xcb_generate_id(connection);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  mask = XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK;
  mask = XCB_CW_BORDER_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_OVERRIDE_REDIRECT |
         XCB_CW_SAVE_UNDER | XCB_CW_EVENT_MASK;

  uint32_t vals[] = {
    screen->black_pixel, XCB_GRAVITY_NORTH_WEST, 0, 1,
    XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS |
      XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_BUTTON_MOTION | XCB_EVENT_MASK_BUTTON_PRESS |
      XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_STRUCTURE_NOTIFY};

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 0,
                    0, static_cast<unsigned short>(window_width),
                    static_cast<unsigned short>(window_height), 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask,
                    vals);
  // set window title
  xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
                      XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, sWindowTitle.size(),
                      sWindowTitle.data());

  if (!window) {
    close_window();
    std::stringstream sError;
    sError << "ERR_XWIN "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* you init the connection and screen_nbr */
  xcb_depth_iterator_t depth_iter;

  depth_iter = xcb_screen_allowed_depths_iterator(screen);
  for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
    xcb_visualtype_iterator_t visual_iter;

    visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
    for (; visual_iter.rem; xcb_visualtype_next(&visual_iter)) {
      if (screen->root_visual == visual_iter.data->visual_id) {
        visual_type = visual_iter.data;
        break;
      }
    }
  }

  // create xcb surface
  surface = cairo_xcb_surface_create(connection, window, visual_type,
                                     window_width, window_height);
  if (!surface) {
    close_window();
    std::stringstream sError;
    sError << "ERR_CAIRO "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  // create cairo context
  cr = cairo_create(surface);
  if (!cr) {
    close_window();
    std::stringstream sError;
    sError << "ERR_CAIRO "
           << "  " << __FILE__ << " " << __func__;
    throw std::runtime_error(sError.str());
  }

  /* Map the window on the screen and flush*/
  xcb_map_window(connection, window);
  xcb_flush(connection);
  window_open = true;

  cairo_surface_flush(surface);

  /// @brief start message monitor and queued block dispatch.
  std::thread message_queue_thread([=]() {
    bProcessing = true;
    message_loop();
  });

  message_queue_thread.detach();

  /// @brief start queue dispatcher. THis uses a condition variable while blocks
  /// are input. messages are processed here serially and filtered.
  std::thread event_queue_thread([=]() {
    bProcessing = true;
    event_queue_processor();
  });

  event_queue_thread.detach();
}

/**
 * @internal
 * @fn close_window
 * @brief closes a window on the target OS
 */
void uxdevice::os_xcb_linux_t::close_window(void) {

  if (surface) {
    cairo_surface_destroy(surface);
    surface = nullptr;
  }

  if (cr) {
    cairo_destroy(cr);
    cr = nullptr;
  }

  if (graphics) {
    xcb_free_gc(connection, graphics);
    graphics = 0;
  }

  if (window) {
    xcb_destroy_window(connection, window);
    window = 0;
  }

  if (xdisplay) {
    XCloseDisplay(xdisplay);
    xdisplay = nullptr;
  }

  window_open = false;
}

/**
 * @fn void flush(void)
 * @brief completes cairo surface and flushes the xcb connection
 *
 */
void uxdevice::os_xcb_linux_t::flush_window(void) {
  if (connection)
    xcb_flush(connection);
}

/**
 * @var message_dispatch_t
 * @brief translates between system domain.
 * generic_os_event_queue_message_t is defined in compile options as it is
 * associated with os compile target.
 */
const uxdevice::message_dispatch_t<xcb_generic_event_t *>
  uxdevice::os_xcb_linux_t::message_dispatch = {
    {XCB_KEY_PRESS,
     [](auto wm, auto e) {
       wm->dispatch<xcb_keyboard_device_t *, xcb_key_press_event_t *>(e);
     }},

    {XCB_KEY_RELEASE,
     [](auto wm, auto e) {
       wm->dispatch<xcb_keyboard_device_t *, xcb_key_release_event_t *>(e);
     }},

    {XCB_BUTTON_PRESS,
     [](auto wm, auto e) {
       wm->dispatch<xcb_mouse_device_t *, xcb_button_press_event_t *>(e);
     }},

    {XCB_BUTTON_RELEASE,
     [](auto wm, auto e) {
       wm->dispatch<xcb_mouse_device_t *, xcb_button_release_event_t *>(e);
     }},

    {XCB_MOTION_NOTIFY,
     [](auto wm, auto e) {
       wm->dispatch<xcb_mouse_device_t *, xcb_motion_notify_event_t *>(e);
     }},

    {XCB_EXPOSE,
     [](auto wm, auto e) {
       wm.dispatch<xcb_window_service_t *, xcb_expose_event_t *>(e);
     }},

    {XCB_CONFIGURE_NOTIFY,
     [](auto wm, auto e) {
       wm.dispatch<xcb_window_service_t *, xcb_configure_notify_event_t *>(e);
     }},

    {XCB_CLIENT_MESSAGE, [](auto wm, xcb_generic_event_t *e) {
       wm.dispatch<xcb_window_service_t *, xcb_client_message_event_t *>(e);
     }}};

/**
 * @internal
 * @fn message_loop
 * @brief the routine handles the message processing for the specific operating
 * system. The function is called from processEvents.
 * @details The entry point that processes messages from the operating system
 * application level services. Typically on Linux this is a coupling of xcb and
 * keysyms library for keyboard. Previous incarnations of technology such as
 * this typically used xserver. However, XCB is the newer form. Primarily
 * looking at the code of such programs as vlc, the routine simply places pixels
 * into the memory buffer. while on windows the direct x library is used in
 * combination with windows message queue processing.
 */

void uxdevice::os_xcb_linux_t::message_loop(void) {
  xcb_generic_event_t *xcbEvent;

  /** @brief this waits until the bprocessing flag is turned on and the
   * connection is set.*/
  while (bProcessing && !connection)
    std::this_thread::sleep_for(std::chrono::milliseconds(60));

  // error trap for processing but  no connection?
  if (!connection)
    return;

  // setup close window event
  cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
  reply = xcb_intern_atom_reply(connection, cookie, 0);

  cookie2 = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
  reply2 = xcb_intern_atom_reply(connection, cookie2, 0);

  xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, (*reply).atom,
                      4, 32, 1, &(*reply2).atom);

  /** @brief wait for an event, gather it. as well, gather all messages that may
   * be waiting. Filter duplicate messages that may be received due to
   * continuous input. When all have been gathered, signal the queue processor
   * to translate these messages into uxdevice versions.*/
  while (bProcessing && (xcbEvent = xcb_wait_for_event(connection))) {
    {
      std::lock_guard lock(xcb_event_queue_mutex);
      xcb_event_queue.emplace_back(xcbEvent);
      bvideo_output = false;

      // qt5 does this, it queues all of the input messages at once.
      // this makes the processing of painting and reading input faster.
      while (bProcessing && (xcbEvent = xcb_poll_for_queued_event(connection)))
        xcb_event_queue.emplace_back(xcbEvent);
    }

    xcb_event_queue_condition_variable.notify_one();
  }
}

/**
 * @fn void event_queue_processor(void)
 * @brief thread dispatches queued messages from from window manager. This
 * thread processes these filter message when a block is received. Perhaps one,
 * or more. At times the processing style saves and reflects screen changes
 * quicker due to the condition variable and queued approach.
 *
 */
void uxdevice::os_xcb_linux_t::event_queue_processor(void) {
  while (bProcessing) {
    bool bdone = {};

    // wait until messages are queued and the signal is received.
    std::unique_lock<std::mutex> lk(xcb_event_queue_condition_mutex);
    xcb_event_queue_condition_variable.wait(lk);
    lk.unlock();

    {
      std::lock_guard lock(xcb_event_queue_mutex);
      if (xcb_event_queue.empty())
        bdone = true;
    }
    xcb_generic_event_t *e = {};

    while (!bdone) {
      // get message
      {
        std::lock_guard lock(xcb_event_queue_mutex);
        e = xcb_event_queue.front();
        xcb_event_queue.pop_front();
      }

      /** @brief process, depending upon filter, non handled messages may be
       * faster with a bit range check. this can be enhanced with specific
       * numerical representation. xcb internals possibly. The it->second()
       * invocation performs a dispatch using the std::visit function. */
      auto it = message_dispatch.find(e->response_type & ~0x80);
      if (it != message_dispatch.end())
        it->second(dynamic_cast<window_manager_base_t *>(this), e);
      free(e);
      // pop and check for next iteration.
      {
        std::lock_guard lock(xcb_event_queue_mutex);
        if (xcb_event_queue.empty())
          bdone = true;
        else if (!bProcessing)
          bdone = true;
      }
    }
  }
}
