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
 * @file image_storage.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief
 * @details
 */
// clang-format off

#include <base/unit_object.h>
#include <api/coordinate/coordinate.h>
#include <api/image/image.h>

// clang-format on

/// @brief default
/// constructor
uxdevice::image_block_storage_t::image_block_storage_t()
    : pipeline_memory_t{}, description{}, image_block{} {}

uxdevice::image_block_storage_t::image_block_storage_t(
    const std::string &_description)
    : description(_description) {}

/// @brief copy constructor
uxdevice::image_block_storage_t::image_block_storage_t(
    const image_block_storage_t &other)
    : hash_members_t(std::move(other)), system_error_t(other),
      display_visual_t(other), pipeline_memory_t(other),
      description(other.description), image_block(other.image_block) {}

/// @brief move constructor
uxdevice::image_block_storage_t::image_block_storage_t(
    image_block_storage_t &&other) noexcept
    : hash_members_t(std::move(other)), system_error_t(other),
      display_visual_t(std::move(other)), pipeline_memory_t(std::move(other)),
      description(std::move(other.description)),
      image_block(std::move(other.image_block)) {}

/// @brief copy assignment
/// operator
uxdevice::image_block_storage_t &
uxdevice::image_block_storage_t::operator=(const image_block_storage_t &other) {
  hash_members_t::operator=(other);
  system_error_t::operator=(other);
  display_visual_t::operator=(other);
  pipeline_memory_t::operator=(other);
  description = other.description;
  image_block = other.image_block;
  return *this;
}

/// @brief move assignment
uxdevice::image_block_storage_t &uxdevice::image_block_storage_t::operator=(
    image_block_storage_t &&other) noexcept {
  hash_members_t::operator=(other);
  system_error_t::operator=(other);
  display_visual_t::operator=(other);
  pipeline_memory_t::operator=(other);
  description = std::move(other.description);
  image_block = std::move(other.image_block);
  return *this;
}

uxdevice::image_block_storage_t::~image_block_storage_t() {}

bool uxdevice::image_block_storage_t::is_valid(void) {
  return image_block.is_valid();
}

std::size_t uxdevice::image_block_storage_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(image_block_storage_t)),
               description, pipeline_memory_hash_code());

  return __value;
}

/**
 * @internal
 * @fn pipeline_acquire
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief create the image rendering pipeline
 * @details The cached rendering function has not been established, create the
 * image rendering pipeline consisting of individualized lambda functions. This
 * functionality can provide image processing capabilities to produce visual
 * effects. Using the emplace back to invoke operations on the image->block_ptr;
 */
void uxdevice::image_block_storage_t::pipeline_acquire() {

  /** compute pipeline that includes rendering commands. The rendering commands
   * are sequenced and appropriate fill, preserve order is maintained.*/
  pipeline_push_visit<fn_emit_cr_t, fn_emit_cr_a_t>();

  /** @brief additional processing can also be inserted here. or may be inserted
   * as a new unit_memory emitter accepting a visitor type.*/

  // add result to buffer
  pipeline_push<order_render>(fn_emit_cr_a_t{
      [&](cairo_t *cr, coordinate_t *a) { image_block.emit(cr, a); }});
}

/**
 * @internal
 * @fn image_block_storage_t::pipeline_has_required_linkages
 * @brief determines if the pipeline has the appropriate linkages or parameters
 * if you will. A validation function.
 */
bool uxdevice::image_block_storage_t::pipeline_has_required_linkages() {
  bool has_requirements = true;

  // validate required parameters
  if (!pipeline_memory_access<coordinate_t>() || description.size() == 0) {
    const char *s = "An image_block_t object must include the following "
                    "attributes. coordinate_t and a description name.";
    error_report(s);
    has_requirements = false;
  }

  return has_requirements;
}
