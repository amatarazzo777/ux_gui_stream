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
 * @file cairo_function.h
 * @date 9/19/20
 * @version 1.0
 * @brief
 */

#pragma once

namespace uxdevice {

/**
 * @typedef cairo_function_t
 * @brief holds a call to a cairo function with parameters bound.
 * The cairo context - cr is provided later.
 */
typedef std::function<void(cairo_t *cr)> cairo_function_t;

/**
 * @internal
 * @brief template specializes the cairo_function_t
 */
template <> struct std::hash<uxdevice::cairo_function_t> {
  std::size_t operator()(uxdevice::cairo_function_t const &o) const noexcept {
    return reinterpret_cast<std::size_t>(std::addressof(o));
  }
};
} // namespace uxdevice
