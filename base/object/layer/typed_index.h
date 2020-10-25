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
@file typed_index.h
@date 5/12/20
@version 1.0
@details

*/

namespace uxdevice {
/**
 * @internal
 * @class typed_index_t
 * @tparam T unit type.
 * @brief class wraps non template key_storage to provide continuation syntax
 * within the object creation parameters.
 */
template <typename T>
class typed_index_t : public key_storage_t, virtual public hash_members_t {
public:
  /// @brief default constructor
  typed_index_t() : key_storage_t{} {}

  /// @brief copy assignment operator
  typed_index_t &operator=(const typed_index_t &other) {
    hash_members_t::operator=(other);
    key_storage_t::operator=(other);
    return *this;
  }
  /// @brief move assignment
  typed_index_t &operator=(typed_index_t &&other) noexcept {
    hash_members_t::operator=(other);
    key_storage_t::operator=(other);
    return *this;
  }
  /// @brief move constructor
  typed_index_t(typed_index_t &&other) noexcept
      : hash_members_t(other), key_storage_t(other) {}

  /// @brief copy constructor
  typed_index_t(const typed_index_t &other) : key_storage_t(other) {}

  /**
   * @internal
   * @fn T index&(const std::string&)
   * @brief returns reference to requested type. This simply provides a
   * continuation syntax when using the obj.index() method within the <<
   * operator or "in" template function.
   *
   * @param _k
   * @return T &
   */
  T &index(const std::string &_k) {
    key_storage_t::operator=(indirect_index_storage_t(_k));
    return *dynamic_cast<T *>(this);
  }

  /**
   * @internal
   * @fn T index&(const std::string&)
   * @brief returns reference to requested type. This simply provides a
   * continuation syntax when using the obj.index() method within the <<
   * operator or "in" template function.
   *
   * @param _k
   * @return T &
   */
  T &index(const std::size_t &_k) {
    key_storage_t::operator=(indirect_index_storage_t(_k));
    return *dynamic_cast<T *>(this);
  }
  /**
   * @fn std::size_t hash_code(void)const
   * @brief hash code of object.
   *
   * @return std::size_t
   */
  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(typed_index_t)), key);
    return __value;
  }
  virtual ~typed_index_t() {}
};
} // namespace uxdevice
