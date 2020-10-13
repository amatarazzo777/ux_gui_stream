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
 * @file ux_image_block.hpp
 * @date 9/23/20
 * @version 1.0
 * @brief
 *  API objects
 *  The following definitions are the exact name that appears within the API.
 * These class templates provide factories that create objects compatible with
 * the system. These objects might have invoke and emit methods depending upon
 * the emit abstract interfaces named for inheritance. The base services for
 * these objects to be functional for the display are provided. These objects
 * have all of the move and copy operators implemented for ease and efficiency
 * of use. There are several types of objects that may be created and inserted
 * by using the template parameters. Please refer to the class template for a
 * more detailed explanation. The following templates are provided for use:
 *
 * 		marker_emitter_t
 * 		painter_brush_emitter_t
 * 		storage_emitter_t,
 * 		class_storage_emitter_t
 */

#include <ux_base.h>
#include <ux_error.h>
#include <ux_visitor_interface.h>
#include <ux_hash.h>

#include <ux_enums.h>

#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>

#include <ux_pipeline_memory.h>

#include <ux_display_visual.h>
#include <ux_display_context.h>
#include <ux_display_unit_base.h>

#include <ux_coordinate.h>

namespace uxdevice {

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
  image_block_storage_t() : pipeline_memory_t{}, description{}, image_block{} {}

  image_block_storage_t(const std::string &_description)
    : description(_description) {}
  /// @brief copy constructor
  image_block_storage_t(const image_block_storage_t &other)
    : hash_members_t(std::move(other)), system_error_t(other),
      display_visual_t(other), pipeline_memory_t(other),
      description(other.description), image_block(other.image_block) {}

  /// @brief move constructor
  image_block_storage_t(image_block_storage_t &&other) noexcept
    : hash_members_t(std::move(other)), system_error_t(other),
      display_visual_t(std::move(other)), pipeline_memory_t(std::move(other)),
      description(std::move(other.description)),
      image_block(std::move(other.image_block)) {}

  /// @brief copy assignment
  /// operator
  image_block_storage_t &operator=(const image_block_storage_t &other) {
    hash_members_t::operator=(other);
    system_error_t::operator=(other);
    display_visual_t::operator=(other);
    pipeline_memory_t::operator=(other);
    description = other.description;
    image_block = other.image_block;
    return *this;
  }

  /// @brief move assignment
  image_block_storage_t &operator=(image_block_storage_t &&other) noexcept {
    hash_members_t::operator=(other);
    system_error_t::operator=(other);
    display_visual_t::operator=(other);
    pipeline_memory_t::operator=(other);
    description = std::move(other.description);
    image_block = std::move(other.image_block);
    return *this;
  }

  virtual ~image_block_storage_t() {}

  bool is_valid(void) { return image_block.is_valid(); }

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(image_block_storage_t)),
                 description, pipeline_memory_hash_code());

    return __value;
  }

  void pipeline_acquire();
  bool pipeline_has_required_linkages(void);

  std::string description = {};
  draw_buffer_t image_block = {};
};

/**
 * @class image_block_t
 * @brief
 */
class image_block_t
  : public class_storage_emitter_t<
      image_block_t, image_block_storage_t,
      accepted_interfaces_t<abstract_emit_context_t<order_render>>> {
public:
  using class_storage_emitter_t::class_storage_emitter_t;

  void emit(display_context_t *context);
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::image_block_storage_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::image_block_t)
