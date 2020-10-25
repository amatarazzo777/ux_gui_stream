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
@file indirect_index.h
@date 5/12/20
@version 1.0
@details

*/
namespace uxdevice {

/**
@internal
@typedef indirect_index_display_unit_t as a base member, the key data member
uses this type. All classes can provide an indirect numerical or string key.
The [] operator exposes searching for objects.
 */
typedef std::variant<std::monostate, std::string, std::size_t>
    indirect_index_storage_t;

/**
 * @internal
 * @brief template specializes the indirect_index_display_unit_t
 */
template <> struct std::hash<uxdevice::indirect_index_storage_t> {
  std::size_t
  operator()(uxdevice::indirect_index_storage_t const &o) const noexcept {
    size_t value = o.index();
    uxdevice::hash_combine(value, std::type_index(typeid(o)).hash_code());
    if (auto pval = std::get_if<std::string>(&o))
      uxdevice::hash_combine(value, *pval);
    else if (auto pval = std::get_if<std::size_t>(&o))
      uxdevice::hash_combine(value, *pval);
    return value;
  }
};

} // namespace uxdevice
