#pragma once

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
 * @file font_storage.h
 * @date 9/7/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {

/**
 * @internal
 * @class text_font_data_storage
 * @brief storage for fonts.@
 * @details
 */
class text_font_storage_t : virtual public hash_members_t {
public:
  text_font_storage_t();

  text_font_storage_t(const std::string &_description);

  /// @brief move constructor
  text_font_storage_t(text_font_storage_t &&other) noexcept;

  /// @brief copy constructor
  text_font_storage_t(const text_font_storage_t &other);

  virtual ~text_font_storage_t();

  text_font_storage_t &operator=(const text_font_storage_t &&other) noexcept;

  text_font_storage_t &operator=(const text_font_storage_t &other);

  text_font_storage_t &operator=(const std::string &_desc);

  /// @brief move assignment
  text_font_storage_t &operator=(const std::string &&_desc) noexcept;

  std::size_t hash_code(void) const noexcept;

  std::string description = {};
  PangoFontDescription *font_ptr = {};
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::text_font_storage_t)
