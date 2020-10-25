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
@file class_storage_emitter.h
@date 5/12/20
@version 1.0
@details

*/
namespace uxdevice {

/**
}
 * @internal
 * @class class_storage_emitter_t
 * @brief provides the flexibility to create classes that store associated data.
 *
 * @tparam T - the name the display unit should assume.
 *
 * @tparam TC - the storage class type. The storage type is publicly inherited
 * and all constructors are also inherited. The class must include the
 * hash_members_t interface. That is, implement the hash_code() member function.
 *
 * @tparam Args... - inherits the data class interface as public. There
 * constructors are inherited as well as the public methods of the class.
 *
 */
template <typename T, typename TC, typename... Args>
class class_storage_emitter_t : public TC,
                                public typed_index_t<T>,
                                public display_unit_t,
                                virtual public hash_members_t,
                                public Args...,
                                public system_base_t {
public:
  using TC::TC;
  class_storage_emitter_t() {}
  virtual ~class_storage_emitter_t() {}

  /// @brief copy constructor
  class_storage_emitter_t(const class_storage_emitter_t &other)
      : hash_members_t(other), TC(other), typed_index_t<T>(other),
        display_unit_t(other) {}

  /// @brief move constructor
  class_storage_emitter_t(class_storage_emitter_t &&other) noexcept
      : hash_members_t(other), TC(other), typed_index_t<T>(other),
        display_unit_t(other) {}

  /// @brief copy assignment operator
  class_storage_emitter_t &operator=(const class_storage_emitter_t &other) {
    display_unit_t::operator=(other);
    hash_members_t::operator=(other);
    typed_index_t<T>::operator=(other);
    TC::operator=(other);
    return *this;
  }

  /// @brief move assignment operator
  class_storage_emitter_t &operator=(class_storage_emitter_t &&other) noexcept {
    display_unit_t::operator=(other);
    hash_members_t::operator=(other);
    typed_index_t<T>::operator=(other);
    TC::opterator = (other);
    return *this;
  }

  /**
   * @fn std::size_t hash_code(void)const
   * @brief
   *
   * @return
   */
  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, typed_index_t<T>::hash_code(),
                 std::type_index(typeid(T)).hash_code(),
                 display_unit_t::hash_code(), TC::hash_code());

    return __value;
  }
};
} // namespace uxdevice
