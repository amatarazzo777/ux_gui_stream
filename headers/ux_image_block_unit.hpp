/*
 * This file is part of the PLATFORM_OBJ distribution
 * {https://github.com/amatarazzo777/platform_obj). Copyright (c) 2020 Anthony
 * Matarazzo.
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
\author Anthony Matarazzo
\file ux_image_block.hpp
\date 9/23/20
\version 1.0
\brief
*/

/********************************************************************************

 API objects

 *********************************************************************************

 The following definitions are the exact name that appears within the API. These
 class templates provide factories that create objects compatible with the
 system. These objects might have invoke and emit methods depending upon the
 emit abstract interfaces named for inheritance. The base services for these
 objects to be functional for the display are provided. These objects have all
 of the move and copy operators implemented for ease and efficiency of use.
 There are several types of objects that may be created and inserted by using
 the template parameters. Please refer to the class template for a more detailed
 explanation. The following templates are provided for use:


 marker_emitter_t
 painter_brush_emitter_t
 storage_emitter_t
 class_storage_emitter_t
 storage_emitter_t
 class_storage_emitter_t


 *********************************************************************************/

/**

 \class image_block_storage_t
 \brief storage class used by the image_block_t object. The oject is responsible
 for encapsulating and dynamically allocating, and releasing memory.

 \details


 */
namespace uxdevice {
class image_block_storage_t : virtual public hash_members_t,
                              virtual public system_error_t,
    : virtual public display_visual_t<visitor_pipeline_memory_image_block_t> {
public:
  /// @brief default
  /// constructor
  image_block_storage_t() : description{}, image_block_ptr{} {}

  image_block_storage_t(const std::string &_description)
      : description(_description) {}

  /// @brief move assignment
  image_block_storage_t &operator=(image_block_storage_t &&other) noexcept {
    description = std::move(other.description);
    image_block = std::move(other.image_block);

    return *this;
  }

  /// @brief copy assignment
  /// operator
  image_block_storage_t &operator=(const image_block_storage_t &other) {
    description = other.description;
    image_block = other.image_block;

    return *this;
  }

  /// @brief move constructor
  image_block_storage_t(image_block_storage_t &&other) noexcept
      : description(std::move(other.description)),
        image_block(std::move(other.image_block)) {}

  /// @brief copy constructor
  image_block_storage_t(const image_block_storage_t &other)
      : description(other.description), image_block(other.image_block) {}

  virtual ~image_block_storage_t() {}

  bool is_valid(void) { return image_block != nullptr; }

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(image_block_storage_t)),
                 description, is_loaded, unit_memory_hash_code());

    return __value;
  }

  std::list<pipeline_t> image_render_pipeline = {};
  pipeline_t pipeline_fn = {};
  void pipeline(cairo_t *cr, coordinate_t *a);

  std::string description = {};
  draw_buffer image_block = {};
  matrix_t matrix = {};
};
} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::image_block_storage_t);

/**
 \class
 \brief
 */
namespace uxdevice {
using image_block_t = class image_block_t
    : public class_storage_emitter_t<image_block_t, image_block_storage_t,
                                     display_visual_t,
                                     emit_display_context_abstract_t> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(display_context_t *context);
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::image_block_t);
