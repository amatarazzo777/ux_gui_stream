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
 * @file ux_display_visual.hpp
 * @date 9/7/20
 * @version 1.0
 * @details The display_visual_t object provides communication
 * between the display context and a display render unit. The object
 * which derives this interface should do it publicly. Another portion of the
 * construct is the rendering pipeline. Two abstract virtual functions must be
 * implemented as they are used by the display_context_t: pipeline_acquire() and
 * pipeline_has_required_linkages(). See ux_pipeline_memory.hpp.
 *
 */

#pragma once

namespace uxdevice {

class context_cairo_region_t {
public:
  context_cairo_region_t();
  context_cairo_region_t(bool bOS, int x, int y, int w, int h);
  context_cairo_region_t(std::size_t _obj, int x, int y, int w, int h);
  context_cairo_region_t(const context_cairo_region_t &other);
  ~context_cairo_region_t();

  cairo_rectangle_int_t rect = {};
  cairo_rectangle_t _rect = {};
  cairo_region_t *_ptr = {};
  std::size_t obj = {};
  bool bOSsurface = {};
};
} // namespace uxdevice
