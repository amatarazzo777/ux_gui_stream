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

#pragma once

namespace uxdevice {

/**
 * @class window_manager_abstract_t
 * @brief class that all target os window manager must inherit.
 *
 */
class window_manager_abstract_t {
public:
  void message_loop(void);
  void open_window(const coordinate_list_t &coord,
                   const std::string &sWindowTitle,
                   const painter_brush_t &background,
                   const event_handler_t &dispatch_events);
  void close_window(void);
};

/**
 * @internal
 * @class os_xcb_linux_t
 * @brief defines the window manager for xcb linux for cairo invocation and
 * surface building.
 *
 */
class os_xcb_linux_t : public window_manager_abstract_t {
public:
  void message_loop(void);
  void open_window(const coordinate_list_t &coord,
                   const std::string &sWindowTitle,
                   const painter_brush_t &background,
                   const event_handler_t &dispatch_events);
  void close_window(void);
};
} // namespace uxdevice
