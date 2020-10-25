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
 * @file hash_interface.h
 * @date 9/19/20
 * @version 1.0
 * @brief hash implementation and virtual interface for function. Macros are
 * defined for ease of implementation across entire framework.
 */

namespace uxdevice {

/**
 * @internal
 * @class hash_members_t
 * @param ... - variadic parameter expanding to hash combine each listed.
 * @brief Creates interface routines for the hashing system and change detection
 * logic. Hashes each of the listed values within the  parameters.
 */
class hash_members_t {
public:
  hash_members_t() {}
  virtual ~hash_members_t() {}
  virtual std::size_t hash_code(void) const noexcept = 0;
  std::size_t __used_hash_code = {};
  void state_hash_code(void) { __used_hash_code = hash_code(); }
  bool is_different_hash() { return hash_code() != __used_hash_code; }
};

/**
 * @fn hash_combine
 * @tparam T class
 * @tparam ... - recursive invocation of self.
 * @details A useful template function described at
 * https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
 */
template <typename T, typename... Rest>
void hash_combine(std::size_t &seed, const T &v, const Rest &... rest) {
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  (hash_combine(seed, rest), ...);
}

} // namespace uxdevice

/**
 * @def
 * @brief allows std::hash<> to select the named class.
 */
#define UX_REGISTER_STD_HASH_SPECIALIZATION(CLASS_NAME)                        \
  template <> struct std::hash<CLASS_NAME> {                                   \
    std::size_t operator()(CLASS_NAME const &o) const noexcept {               \
      return o.hash_code();                                                    \
    }                                                                          \
  };
