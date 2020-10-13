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
 * @file uxdisplayunits.hpp
 * @date 9/7/20
 * @version 1.0
 * @brief The file encapsulates the objects that may be created, controlled, and
 * rendered by the system. The system provides for std::shared pointers to these
 * object to be shared within the rendering cycle. Each object has a hash
 * function which is queried to detect changes within the underlying data. Only
 * on screen related objects are included within the set. The intersection test
 * is included as part of a base hierarchy of classes for objects that are
 * declared as such. The class objects are separated into two distinct entity
 * names. One that is the API name and a data storage class.
 *
 * 	painter_brush_emitter_t
 * 	marker_emitter_t
 * 	storage_emitter_t
 * 	class_storage_emitter_t
 *
 */
#include <ux_base.h>
#include <ux_error.h>
#include <ux_hash.h>
#include <ux_enums.h>

#include <ux_visitor_interface.h>
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
 * @class surface_area_brush_t
 * @brief stores and emits the background of the surface. The class inherits
 * from painter_brush_emitter_t.
 */
class surface_area_brush_t
  : public painter_brush_emitter_t<
      surface_area_brush_t,
      accepted_interfaces_t<abstract_emit_context_t<order_init>>> {
public:
  using painter_brush_emitter_t::painter_brush_emitter_t;

  void emit(display_context_t *context);
};

/**
 * @class surface_area_title_t
 * @brief
 */
class surface_area_title_t
  : public storage_emitter_t<
      surface_area_title_t, std::string,
      accepted_interfaces_t<abstract_emit_context_t<order_init>>> {
public:
  using storage_emitter_t::storage_emitter_t;

  void emit(display_context_t *context);
};

} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::surface_area_brush_t)
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::surface_area_title_t)
