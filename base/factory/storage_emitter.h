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
@file storage_emitter.h
@date 5/12/20
@version 1.0
@details

*/
namespace uxdevice {

}
/**
 * @internal
 * @class storage_emitter_t
 *
 * @tparam T - the name the display unit should assume.
 *
 * @tparam TS - the storage class or trivial type.
 *
 * @tparam Args... - lists applicable invoke and emit functions. Depending on
 * the particular display unit type, context, pango or cairo emit functions may
 * be included from the abstract base functions. When these are listed, the
 * interface callback must be defined.
 *
 * @brief provides the flexibility to store associated trivial data such as
 * enum, double, etc. The template names this storage area within the object
 * 'value'. This can be accessed and controlled from the emit function. A
 * default constructor is established which zero initializes the value. As well,
 * a constructor accepting the initial value of the given storage  type.
 *
 */
template <typename T, typename TS, typename... Args>
class storage_emitter_t : virtual public hash_members_t,
                          public display_unit_t,
                          public typed_index_t<T>,
                          public Args...,
                          public system_base_t {
public:
  storage_emitter_t() : value(TS{}) {}
  storage_emitter_t(const TS &o) : value(o) {}
  virtual ~storage_emitter_t() {}

  /// @brief copy constructor
  storage_emitter_t(const storage_emitter_t &other)
      : hash_members_t(other), system_error_t(other),
        display_unit_t(other), typed_index_t<T>(other), value(other.value) {}

  /// @brief move constructor
  storage_emitter_t(storage_emitter_t &&other) noexcept
      : hash_members_t(other), system_error_t(other),
        display_unit_t(other), typed_index_t<T>(other), value(other.value) {}

  /// @brief copy assignment operator
  storage_emitter_t &operator=(const storage_emitter_t &other) {
    hash_members_t::operator=(other);
    system_error_t::operator=(other);
    display_unit_t::operator=(other);
    typed_index_t<T>::operator=(other);
    value = other.value;
    return *this;
  }

  /// @brief move assignment operator
  storage_emitter_t &operator=(storage_emitter_t &&other) noexcept {
    hash_members_t::operator=(other);
    system_error_t::operator=(other);
    display_unit_t::operator=(other);
    typed_index_t<T>::operator=(other);
    value = other.other;
    return *this;
  }
  /**
   * @fn std::size_t hash_code(void)const
   * @brief hash of object
   *
   * @return
   */
  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, typed_index_t<T>::hash_code(),
                 std::type_index(typeid(T)).hash_code(),
                 display_unit_t::hash_code(), value);

    return __value;
  }
  TS value;
};
}
