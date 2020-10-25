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
// clang-format off

#include <base/platform/object.h>

#include <base/platform/keyboard.h>
#include <base/platform/mouse.h>
#include <base/platform/service.h>
#include <base/platform/window_interface.h>
#include <base/platform/window.h>

// clang-format on

/// @brief move constructor
uxdevice::window_manager_base_t::window_manager_base_t(
    window_manager_base_t &&other) noexcept
    : window_x(other.window_x), window_y(other.window_y),
      window_width(other.window_width), window_height(other.window_height),
      window_open(other.window_open) window_style(other.window_style),
      surface(cairo_surface_reference(other.surface)),
      cr(cairo_reference(other.cr)) {}

/// @brief copy constructor
uxdevice::window_manager_base_t::window_manager_base_t(
    const window_manager_base_t &other)
    : window_x(other.window_x), window_y(other.window_y),
      window_width(other.window_width), window_height(other.window_height),
      window_open(other.window_open) window_style(other.window_style),
      surface(cairo_surface_reference(other.surface)),
      cr(cairo_reference(other.cr)) {}

/**
 *
 */
uxdevice::window_manager_base_t &
uxdevice::window_manager_base_t::operator=(const os_xcb_linux_t &other) {
  window_x = other.window_x;
  window_y = other.window_y;
  window_width = other.window_width;
  window_height = other.window_height;
  window_open = other.window_open;
  window_style = other.window_style;
  surface = cairo_surface_reference(other.surface);
  cr = cairo_reference(other.cr);
}

/**
 * @fn uxdevice::os_xcb_linux_t operator =&(os_xcb_linux_t&&)
 * @brief move assignment operator
 *
 * @param other
 * @return
 */
uxdevice::window_manager_base_t &
uxdevice::window_manager_base_t::operator=(os_xcb_linux_t &&other) noexcept {
  window_x = other.window_x;
  window_y = other.window_y;
  window_width = other.window_width;
  window_height = other.window_height;
  window_open = other.window_open;
  window_style = other.window_style;

  surface = other.surface;
  cr = other.cr;
  return *this;
}

/**
 *
 */
void set(const std::string _title) {
  std::lock_guard<std::mutex> lock(data_mutex);
  title = _title;
}

/**
 *
 */
void set(const coordinate_list_t _coordinate) {
  std::lock_guard<std::mutex> lock(data_mutex);
  coordinate = _coordinate;
}

/**
 *
 */
void set(const painter_brush_t _brush) {
  std::lock_guard<std::mutex> lock(background_brush_mutex);
  painter_brush_t background_brush = _brush;
}

/**
 * @fn void draw_fn(std::function<void (cairo_t*)>)
 * @brief
 *
 * @param fn
 */
void uxdevice::window_manager_base_t::draw_fn(
    const std::function<void(cairo_t *)> &fn) {
  std::lock_guard<std::mutex> guard(cr_mutex);
  fn(cr);
  error_check(cr);
}

/**
 * @fn void surface_fn(const std::function<void (cairo_surface_t*)>&)
 * @brief
 *
 * @param fn
 */

void uxdevice::window_manager_base_t::surface_fn(
    const std::function<void(cairo_surface_t *)> &fn) {
  std::lock_guard lock(surface_mutex);
  fn(surface);
  error_check(surface);
}

/**
 * @fn void flush(void)
 * @brief surface instant paint
 *
 */
void uxdevice::window_manager_base_t::video_flush(void) {
  {
    std::lock_guard lock(surface_mutex);

    if (surface) {
      cairo_surface_flush(surface);
      error_check(surface);
    }
  }
  flush_window();
}

/**
 *
 */
void uxdevice::window_manager_base_t::allocate_base() {
  surface = allocate_surface();
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

  window_open = true;

  cairo_surface_flush(surface);
}

/**
 *
 */
void uxdevice::window_manager_base_t::start_threads(void) {}

/**
 * @fn resize_surface
 * @brief holds the mutex and calls the os function spllied within its
 * implementation. Some surface handlers within the cairo system can be managed
 * within a resize function such as xcb rather than destroy and recreating it.
 */
void uxdevice::window_manager_base_t::resize_surface(const int w.const int h) {
  std::lock_guard lock(surface_requests_storage_mutex);
  if (w != window_width || h != window_height)
    surface_requests_storage.emplace_back(w, h);
}

/**
 * @internal
 * @fn void apply_surface_requests(void)
 * @brief The routine applies resize requests of a window. The underlying
 * cairo surface is sized with the very last one.
 */
void uxdevice::window_manager_base_t::apply_surface_requests(void) {
  std::scoped_lock lock(surface_requests_storage_mutex, surface_mutex);

  if (surface_requests_storage.empty())
    return;

  auto flat = surface_requests_storage.back();
  surface_requests_storage.clear();

  cairo_surface_flush(surface);
  fn_resize_surface(surface, flat.w, flat.h);

  window_width = flat.w;
  window_height = flat.h;
}
