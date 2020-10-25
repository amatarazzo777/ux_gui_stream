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
@file painter_brush_emitter.h
@date 5/12/20
@version 1.0
@details

*/

namespace uxdevice {
/**
 * @internal
 * @class painter_brush_emitter_t
 *
 * @tparam T used to note and enable the typed index interface which allows
 * continuation syntax when objects are created and an indirect index is
 * applied. Such as: text_font_t{"arial 20px"}.index("idxfont") Here the .index
 * property returns a strong type of the original.
 *
 * @tparam Args... provides the ability to catch specific functions and their
 * prototypes. emit. Attributes may also be listed that apply to specific
 * functionality of the system such as labeling items as textual rendering
 * related. This is useful when applying attributes that comprise a rendering
 * function.
 *
 * @brief creates a painter brush object that is also a display unit. class
 * inherits publicly display_unit_t and painter_brush_t. The painter_brush_t
 * constructor interface is also inherited which allows multiple types of
 * brushes to be defined in several formats,  */
template <typename T, typename... Args>
class painter_brush_emitter_t : virtual public hash_members_t,
                                public system_base_t,
                                public display_unit_t,
                                public painter_brush_t,
                                public typed_index_t<T>,
                                public Args... {

public:
  using painter_brush_t::painter_brush_t;
  painter_brush_emitter_t() {}

  /// @brief copy constructor
  painter_brush_emitter_t(const painter_brush_emitter_t &other)
      : hash_members_t(other), system_error_t(other), system_base_t(other),
        display_unit_t(other),
        painter_brush_t(other), typed_index_t<T>::typed_index_t(other) {}

  /// @brief move constructor
  painter_brush_emitter_t(painter_brush_emitter_t &&other) noexcept
      : hash_members_t(other), system_error_t(other), system_base_t(other),
        display_unit_t(other),
        painter_brush_t(other), typed_index_t<T>::typed_index_t(other) {}

  /// @brief copy assignment operator
  painter_brush_emitter_t &operator=(const painter_brush_emitter_t &other) {
    hash_members_t::operator=(other);
    system_base_t::operator=(other);
    system_error_t::operator=(other);
    display_unit_t::operator=(other);
    painter_brush_t::operator=(other);
    typed_index_t<T>::operator=(other);

    return *this;
  }
  /// @brief move assignment operator
  painter_brush_emitter_t &operator=(painter_brush_emitter_t &&other) noexcept {
    hash_members_t::operator=(other);
    system_base_t::operator=(other);
    system_error_t::operator=(other);
    display_unit_t::operator=(other);
    painter_brush_t::operator=(other);
    typed_index_t<T>::operator=(other);
    return *this;
  }

  /**
   * @fn  ~painter_brush_emitter_t()
   * @brief
   *
   */
  virtual ~painter_brush_emitter_t() {}

  /**
   * @fn std::size_t hash_code(void)const
   * @brief
   *
   * @return
   */
  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(T)),
                 painter_brush_t::hash_code(), display_unit_t::hash_code(),
                 lineWidth, radius, x, y);

    return __value;
  }

  double lineWidth = 1;
  unsigned short radius = 3;
  double x = 1, y = 1;
};
} // namespace uxdevice
