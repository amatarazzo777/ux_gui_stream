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
@file window_interface.h
@date 10/15/20
@version 1.0
@brief
*/

namespace uxdevice {

/**
 * @brief forward declarations
 */
struct cairo_surface_t;

/**
 *
 */
class window_manager_interface_t {

  ///@brief this is where the interface for several types of windows classes
  /// might go. Such as a pop-up window which obscures the window partially
  /// as well as the border. Or a child window. Settings the transparency of
  /// the window.

  /**
   * @fn void open_window(const coordinate_list_t&, const std::string&,
   * const painter_brush_t&, const event_handler_t&)=0
   * @brief
   *
   */
  virtual void fn_open_window() = 0;

  /**
   * @fn void close_window(void)=0
   * @brief
   *
   */
  virtual void fn_close_window(void) = 0;

  /**
   * @fn void flush_window(void)=0
   * @brief
   *
   */
  virtual void fn_flush_window(void) = 0;

  /**
   * @fn typename T allocate_surface(void)=0
   * @brief provides the inheritance model with a way to be specifi about
   * the type of window and surface tie from the cairo system. That is it
   * does support multiple types of back end rendering.
   *
   */
  virtual cairo_surface_t *fn_allocate_surface(void) = 0;

  /**
   * @fn fn_resize_surface
   * @brief os implementation of cairo surface resize. xcb version provides a
   * specific function.
   */
  virtual void fn_resize_surface(cairo_surface_t *surface, const int w,
                                 const int h) = 0;

  /**
   * @internal
   * @fn fn_set_window_title
   * @brief The function sets the window title, should be implemented within
   * the resulting OS implementation.
   *
   */
  virtual void fn_set_window_title(const std::string &stitle) = 0;
};
} // namespace uxdevice
