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
 * @file image_storage.h
 * @date 10/23/20
 * @version 1.0
 * @brief
 */

namespace uxdevice {
class hash_members_t;
class system_error_t;
class display_visual_t;
class pipeline_memory_t;

/**
 * @class image_block_storage_t
 *
 * @brief storage class used by the image_block_t object. The object is
 * responsible for encapsulating and dynamically allocating, and releasing
 * memory.
 *
 * @details
 *
 */
class image_block_storage_t : virtual public hash_members_t,
                              virtual public system_error_t,
                              public display_visual_t,
                              public pipeline_memory_t {
public:
  /// @brief default
  /// constructor
  image_block_storage_t();

  image_block_storage_t(const std::string &_description);
  /// @brief copy constructor
  image_block_storage_t(const image_block_storage_t &other);

  /// @brief move constructor
  image_block_storage_t(image_block_storage_t &&other) noexcept;

  /// @brief copy assignment
  /// operator
  image_block_storage_t &operator=(const image_block_storage_t &other);

  /// @brief move assignment
  image_block_storage_t &operator=(image_block_storage_t &&other) noexcept;

  virtual ~image_block_storage_t();

  bool is_valid(void);

  std::size_t hash_code(void) const noexcept;

  void pipeline_acquire();
  bool pipeline_has_required_linkages(void);

  std::string description = {};
  draw_buffer_t image_block = {};
};
} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::image_block_storage_t)
