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
#include <api/coordinate/coordinate.h>

// clang-format on

/**
 * @fn default constructor
 * @brief the default constructor needs to be here because of the class
 * forwarding.
 */
uxdevice::display_unit_t::display_unit_t() {}

/// @brief move constructor
uxdevice::display_unit_t::display_unit_t(display_unit_t &&other) noexcept
    : hash_members_t(other), system_error_t(other),
      is_processed(std::move(other.is_processed)) {}

/// @brief copy constructor
uxdevice::display_unit_t::display_unit_t(const display_unit_t &other)
    : hash_members_t(other), system_error_t(other),
      is_processed(other.is_processed) {}

/**
 *
 */
uxdevice::display_unit_t &
uxdevice::display_unit_t::operator=(display_unit_t &&other) {
  hash_members_t::operator=(other);
  system_error_t::operator=(other);
  is_processed = std::move(other.is_processed);
  return *this;
}

/// @brief copy assignment operator
uxdevice::display_unit_t &
uxdevice::display_unit_t::operator=(const display_unit_t &other) {
  hash_members_t::operator=(other);
  system_error_t::operator=(other);
  is_processed = other.is_processed;
  return *this;
}

void uxdevice::display_unit_t::changed(void) { bchanged = true; }

bool uxdevice::display_unit_t::has_changed(void) { return is_different_hash(); }

std::size_t uxdevice::display_unit_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(display_unit_t)), is_processed,
               bchanged);
  return __value;
}
