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
 * @file ux_os_window_manager_base.cpp
 * @date 9/25/20
 * @version 1.0
 * @brief
 * @details
 */
#include <ux_compile_options.h>
#include <ux_base.h>
#include <ux_system_error.h>
#include <ux_os_window_manager_base.h>

/**
 * @fn void draw_fn(std::function<void (cairo_t*)>)
 * @brief
 *
 * @param fn
 */
void uxdevice::window_manager_base_t::draw_fn(
  const std::function<void(cairo_t *)> &fn) {
  std::lock_guard(cr_mutex);
  fn(cr);
  error_check(cr);
}

/**
 * @fn void flush(void)
 * @brief surface instant paint
 *
 */
void uxdevice::window_manager_base_t::video_flush(void) {
  {
    std::lock_guard lock(surface_mutex);
    if (window_manager->surface) {
      cairo_surface_flush(window_manager->surface);
      error_check(window_manager->surface);
    }
  }
  flush_window();
}

/**
 * @fn void surface_fn(const std::function<void (cairo_surface_t*)>&)
 * @brief
 *
 * @param fn
 */

void uxdevice::window_manager_base_t::surface_fn(
  const std::function<void(cairo_surface_t *)> &fn) {
  std::lock_guard(surface_mutex);
  fn(surface);
  error_check(surface);
}
