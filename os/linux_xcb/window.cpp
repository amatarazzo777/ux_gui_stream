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
// clang-format off

#include <base/unit_object.h>


#include <os/linux_xcb/os.h>
#include <os/linux_xcb/window.h>

// clang-format on

using namespace std;
using namespace uxdevice;

uxdevice::os_xcb_linux_t::os_xcb_linux_t() { open_connection(); }
uxdevice::os_xcb_linux_t::~os_xcb_linux_t() { close_connection(); }

/// @brief copy constructor
uxdevice::os_xcb_linux_t::os_xcb_linux_t(const os_xcb_linux_t &other) : {}

/// @brief move constructor
uxdevice::os_xcb_linux_t::os_xcb_linux_t(os_xcb_linux_t &&other) noexcept : {}

/**
 * @fn uxdevice::os_xcb_linux_t operator =&(const os_xcb_linux_t&)
 * @brief copy assignment operator
 *
 * @param other
 * @return
 */
uxdevice::os_xcb_linux_t &
uxdevice::os_xcb_linux_t::operator=(const os_xcb_linux_t &other) {
  window_manager_base_t::operator=(other);

  xdisplay = other.xdisplay;
  connection = other.connection;
  screen = other.screen;
  window = other.window;
  graphics = other.graphics;
  visual_type = other.visual_type;
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
  window_manager_base_t::operator=(other);
  xdisplay = other.xdisplay;
  connection = other.connection;
  screen = other.screen;
  window = other.window;
  graphics = other.graphics;
  visual_type = other.visual_type;

  return *this;
}

/**
 * @internal
 * @fn
 * @brief
 */
void uxdevice::os_xcb_linux_t::open_connection(void) {
  /// @brief if the items have already been initialized by another logic path,
  /// use the ones provided.
  if (xdisplay && connection && graphics)
    return;

  if (!xdisplay = XOpenDisplay(nullptr))
    throw_error_report(__FILE__, const std::size_t ln, __func__,
                       "XOpenDisplay returned null.");

  /* get the connection to the X server */
  if (!connection = XGetXCBConnection(xdisplay))
    throw_error_report(__FILE__, const std::size_t ln, __func__,
                       "XGetXCBConnection returned null.");

  /* Get the first screen */
  screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
  if (!screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data)
    throw_error_report(__FILE__, const std::size_t ln, __func__,
                       "xcb_setup_roots_iterator returned null.");

  /* Create black (foreground) graphic context */
  window = screen->root;
  graphics = xcb_generate_id(connection);
  uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  uint32_t values[] = {screen->black_pixel, 0};
  xcb_create_gc(connection, graphics, window, mask, values);

  if (!graphics)
    throw_error_report(__FILE__, const std::size_t ln, __func__,
                       "xcb_create_gc returned null.");

  /// @brief set the base window manager's settings.
  screen_width = screen->width_in_pixels;
  screen_height = screen->height_in_pixels;
}

/**
 * @internal
 * @fn
 * @brief
 */
void uxdevice::os_xcb_linux_t::close_connection(void) {

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
}

/**
 *
 * @internal
 * @fn fn_open_window

 */
void uxdevice::os_xcb_linux_t::fn_open_window(void) {

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

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root,
                    static_cast<unsigned short>(window_x),
                    static_cast<unsigned short>(window_y), ,
                    static_cast<unsigned short>(window_width),
                    static_cast<unsigned short>(window_height), 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask,
                    vals);

  if (!window)
    throw_error_report(__FILE__, const std::size_t ln, __func__,
                       "XOpenDisplay returned null.");

  /* Map the window on the screen.*/
  xcb_map_window(connection, window);

  // setup close window event
  cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
  reply = xcb_intern_atom_reply(connection, cookie, 0);

  cookie2 = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
  reply2 = xcb_intern_atom_reply(connection, cookie2, 0);

  xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, (*reply).atom,
                      4, 32, 1, &(*reply2).atom);
}

/**
 * @internal
 * @fn allocate_surface
 * @brief implementation of the allocation for surface for the specific os.
 * The other parts of the system are allocated by the base. The pointer here is
 * also managed by the base. That is, the "resource free" is within the base.
 * hence the name of the function "allocate".
 *
 * Create xcb surface,
 * Here the cairo_xcb_surface_create specific function is used.
 *
 */
cairo_surface_t *uxdevice::os_xcb_linux_t::fn_allocate_surface(void) {

  /* find the visual_type. */
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

  return cairo_xcb_surface_create(connection, window, visual_type,
                                       window_width, window_height));
}

/**
 * @internal
 * @fn fn_resize_surface
 * @brief the function provides the size change of the surface which is an os
 * dependent function. This function is called from a multitasking point which
 * the base class manages the locking on the read and write of this object
 * specifically.
 */
uxdevice::os_xcb_linux_t::fn_resize_surface(cairo_surface_t *_s, const int w,
                                            const int h) {
  cairo_xcb_surface_set_size(_s, w, h);
}

/**
 * @internal
 * @fn close_window
 * @brief closes a window on the target OS
 */
void uxdevice::os_xcb_linux_t::close_window(void) { close_connection(); }

/**
 * @fn void flush(void)
 * @brief completes cairo surface and flushes the xcb connection
 *
 */
void uxdevice::os_xcb_linux_t::fn_flush_window(void) {
  if (connection)
    xcb_flush(connection);
}

/**
 * @internal
 * @fn wait_message(void)
 * @brief waits for an event. The system will no return until a message has been
 * received.
 * @return xcb_generic_event_t *
 *
 */
xcb_generic_event_t *uxdevice::os_xcb_linux_t::fn_wait_message(void) {
  return xcb_wait_for_event(connection);
}

/**
 * @internal
 * @fn poll_message(void)
 * @brief polls for an event. retrusn null when non exists. this gives the
 * opportunity to process a batch of them. Starting with a wait, and then
 * calling this function until there are no more.
 * @return xcb_generic_event_t *
 *
 */
xcb_generic_event_t *uxdevice::os_xcb_linux_t::fn_poll_message(void) {
  return xcb_poll_for_queued_event(connection);
}

/**
 * @internal
 * @fn complete_message(xcb_generic_event_t *xcb)
 * @brief frees the event
 * @return void
 */
voiduxdevice::os_xcb_linux_t::fn_complete_message(xcb_generic_event_t *xcb) {
  free(xcb);
}

/**
 * @internal
 * @var message_dispatch_t
 * @brief translates between system domain.
 * generic_os_event_queue_message_t is defined in compile options as it is
 * associated with os compile target. It is defined as a static so only one
 * exists in all versions of the window object.
 */
void uxdevice::os_xcb_linux_t::fn_visit_dispatch(xcb_generic_event_t *xcb) {
  static const message_dispatch_t message_dispatch = {
      {XCB_KEY_PRESS,
       [&]() { dispatch<keyboard_device_xcb_t, key_press_xcb_t>(xcb); }},

      {XCB_KEY_RELEASE,
       [&]() { dispatch<keyboard_device_xcb_t, key_release_xcb_t>(xcb); }},

      {XCB_BUTTON_PRESS,
       [&]() { dispatch<mouse_device_xcb_t, button_press_xcb_t>(xcb); }},

      {XCB_BUTTON_RELEASE,
       [&]() { dispatch<mouse_device_xcb_t, button_release_xcb_t>(xcb); }},

      {XCB_MOTION_NOTIFY,
       [&]() { dispatch<mouse_device_xcb_t, motion_notify_xcb_t>(xcb); }},

      {XCB_EXPOSE,
       [&]() { dispatch<window_service_xcb_t, surface_expose_xcb_t>(xcb); }},

      {XCB_CONFIGURE_NOTIFY,
       [&]() { dispatch<window_service_xcb_t, configure_notify_xcb_t>(xcb); }},

      {XCB_CLIENT_MESSAGE,
       [&]() { dispatch<window_service_xcb_t, client_message_xcb_t>(xcb); }}

  };

  /** @brief process, depending upon filter, non handled messages may be
   * faster with a bit range check. this can be enhanced with specific
   * numerical representation. xcb internals possibly. The it->second()
   * invocation performs a dispatch using the std::visit function. */
  auto it = message_dispatch.find(xcb->response_type & ~0x80);
  if (it != message_dispatch.end())
    it->second();
}

/**
 *
 */
void uxdevice::os_xcb_linux_t::fn_set_window_title(void) {
  // set window title
  xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window,
                      XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, title.size(),
                      title.data());
}
