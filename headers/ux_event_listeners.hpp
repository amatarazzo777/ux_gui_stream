/*
 * This file is part of the PLATFORM_OBJ distribution
 * {https://github.com/amatarazzo777/platform_obj). Copyright (c) 2020 Anthony
 * Matarazzo.
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
\author Anthony Matarazzo
\file ux_evvent_listeners.hpp
\date 9/23/20
\version 1.0
\brief
*/
// event listeners
/**
 \class
 \brief
 */
namespace uxdevice {
using listen_paint_t =
    class listen_paint_t : public listener_t<listen_paint_t> {};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_paint_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_focus_t =
    class listen_focus_t : public listener_t<listen_focus_t> {};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_focus_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_blur_t = class listen_blur_t : public listener_t<listen_blur_t> {};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_blur_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_resize_t =
    class listen_resize_t : public listener_t<listen_resize_t> {};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_resize_t);

/**

 \class
 \brief
 */
namespace uxdevice {
using listen_keydown_t =
    class listen_keydown_t : public listener_t<listen_keydown_t> {};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_keydown_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_keyup_t =
    class listen_keyup_t : public listener_t<listen_keyup_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_keyup_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_keypress_t =
    class listen_keypress_t : public listener_t<listen_keypress_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_keypress_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_mouseenter_t =
    class listen_mouseenter_t : public listener_t<listen_mouseenter_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mouseenter_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_mousemove_t =
    class listen_mousemove_t : public listener_t<listen_mousemove_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mousemove_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_mousedown_t =
    class listen_mousedown_t : public listener_t<listen_mousedown_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mousedown_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_mouseup_t =
    class listen_mouseup_t : public listener_t<listen_mouseup_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mouseup_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_click_t =
    class listen_click_t : public listener_t<listen_click_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_click_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_dblclick_t =
    class listen_dblclick_t : public listener_t<listen_dblclick_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_dblclick_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_contextmenu_t =
    class listen_contextmenu_t : public listener_t<listen_contextmenu_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_contextmenu_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_wheel_t =
    class listen_wheel_t : public listener_t<listen_wheel_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_wheel_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using listen_mouseleave_t =
    class listen_mouseleave_t : public listener_t<listen_mouseleave_t> {
public:
  using listener_t::listener_t;
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::listen_mouseleave_t);
