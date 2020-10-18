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
 * @author Anthony Matarazzo
 * @file ux_event_listeners.h
 * @date 9/23/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {

/**
@internal
@class listener_t

@tparam T - the name the display unit should assume.

@brief


*/
template <typename T>
class listener_t : public typed_index_t<T>, virtual public hash_members_t {
public:
  listener_t() = delete;
  listener_t(event_handler_t _dispatch)
    : ti(std::type_index(typeid(T))), dispatch_event(_dispatch) {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, typed_index_t<T>::hash_code(), ti);
    return __value;
  }

  std::type_index ti;
  event_handler_t dispatch_event;
};

/**
 @class
 @brief
 */
class listen_close_window_t : public listener_t<listen_close_window_t> {
  using listener_t::listener_t;
  window_service_event_t *d = {};
};

/**
 @class
 @brief
 */
class listen_paint_t : public listener_t<listen_paint_t> {
  using listener_t::listener_t;
  window_service_event_t *d = {};
};

/**
 @class
 @brief
 */
class listen_focus_t : public listener_t<listen_focus_t> {
  using listener_t::listener_t;
  window_service_event_t *d = {};
};

/**
 @class
 @brief
 */
class listen_blur_t : public listener_t<listen_blur_t> {
  using listener_t::listener_t;
  window_service_event_t *d = {};
};

/**
 @class
 @brief
 */
class listen_resize_t : public listener_t<listen_resize_t> {
public:
  using listener_t::listener_t;
  window_service_event_t *d = {};
};

/**

 @class
 @brief
 */
class listen_keydown_t : public listener_t<listen_keydown_t> {
public:
  using listener_t::listener_t;
  keyboard_device_event_t *d = {};
};

/**
 @class
 @brief
 */
class listen_keyup_t : public listener_t<listen_keyup_t> {
public:
  using listener_t::listener_t;

public:
  keyboard_device_event_t *d = {};
};

/**
 @class
 @brief
 */
class listen_keypress_t : public listener_t<listen_keypress_t> {
public:
  using listener_t::listener_t;

public:
  keyboard_device_event_t *d = {};
};

/**
 @class
 @brief
 */
class listen_mouseenter_t : public listener_t<listen_mouseenter_t> {
public:
  using listener_t::listener_t;

public:
  mouse_device_event_t *d = {};
};

/**
 @class
 @brief
 */
class listen_mousemove_t : public listener_t<listen_mousemove_t> {
public:
  using listener_t::listener_t;
};

/**
 @class
 @brief
 */
class listen_mousedown_t : public listener_t<listen_mousedown_t> {
public:
  using listener_t::listener_t;
};

/**
 @class
 @brief
 */
class listen_mouseup_t : public listener_t<listen_mouseup_t> {
public:
  using listener_t::listener_t;
};

/**
 @class
 @brief
 */
class listen_click_t : public listener_t<listen_click_t> {
public:
  using listener_t::listener_t;
};

/**
 @class
 @brief
 */
class listen_dblclick_t : public listener_t<listen_dblclick_t> {
public:
  using listener_t::listener_t;
};

/**
 @class
 @brief
 */
class listen_contextmenu_t : public listener_t<listen_contextmenu_t> {
public:
  using listener_t::listener_t;
};

/**
 @class
 @brief
 */
class listen_wheel_t : public listener_t<listen_wheel_t> {
public:
  using listener_t::listener_t;
};

/**
 @class
 @brief
 */
class listen_mouseleave_t : public listener_t<listen_mouseleave_t> {
public:
  using listener_t::listener_t;
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_paint_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_focus_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_blur_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_resize_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_keydown_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_keyup_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_keypress_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mouseenter_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mousemove_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mousedown_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mouseup_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_click_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_dblclick_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_contextmenu_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_wheel_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mouseleave_t)
