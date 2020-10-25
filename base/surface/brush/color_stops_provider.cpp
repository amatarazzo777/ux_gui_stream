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
 * @file color_stops_provider.cpp
 * @date 5/12/20
 * @version 1.0
 * @details
 */

// clang-format off

#include <base/object/layer/hash_interface.h>
#include "color_stop.h"
#include "color_stops_provider.h"

// clang-format on

/// @brief default constructor
uxdevice::color_stops_provider_t::color_stops_provider_t() {}

uxdevice::color_stops_provider_t::~color_stops_provider_t() {}

/// @brief copy constructor
uxdevice::color_stops_provider_t::color_stops_provider_t(
    const color_stops_provider_t &other)
    : hash_members_t(other), color_stops(other.color_stops) {}

/// @brief move constructor
uxdevice::color_stops_provider_t::color_stops_provider_t(
    color_stops_provider_t &&other) noexcept
    : hash_members_t(other), color_stops(other.color_stops) {}

/**
 * @fn color_stops_provider_t operator =&(const color_stops_provider_t&)
 * @brief copy assignment
 *
 * @param other
 * @return
 */
uxdevice::color_stops_provider_t &uxdevice::color_stops_provider_t::operator=(
    const color_stops_provider_t &other) {
  hash_members_t::operator=(other);
  color_stops = other.color_stops;
  return *this;
}

/**
 * @fn color_stops_provider_t operator =&(const color_stops_provider_t&&)
 * @brief move assignment
 *
 * @param other
 * @return
 */
uxdevice::color_stops_provider_t &uxdevice::color_stops_provider_t::operator=(
    color_stops_provider_t &&other) noexcept {
  hash_members_t::operator=(other);
  color_stops = std::move(other.color_stops);
  return *this;
}

/**
 * @internal
 * @fn void resolve(cairo_pattern_t*)
 * @brief  so this logic is used in duel
 * form. When gradients may be named as a string, or provided in complete
 * API form. the logic below fills in the offset values automatically
 * distributing equally across the noted offset. offsets are provided from 0
 * - 1 and name the point within the line.
 *
 * @param pattern
 */
void uxdevice::color_stops_provider_t::resolve_color_stops(
    cairo_pattern_t *pattern) {
  if (!pattern || color_stops.size() == 0)
    return;

  bool bDone = false;
  bool bEdgeEnd = false;

  // first one, if auto offset set to
  //   0 - the beginning of the color stops
  color_stops_iterator_t it = color_stops.begin();
  if (it->bAutoOffset) {
    it->bAutoOffset = false;
    it->offset = 0;
  }
  double dOffset = it->offset;

  while (!bDone) {

    // find first color stop with a defined offset.
    color_stops_iterator_t it2 =
        find_if(it + 1, color_stops.end(),
                [](auto const &o) { return !o.bAutoOffset; });

    // not found, the last item in color stops did not have a value,
    // assign it 1.0
    if (it2 == color_stops.end()) {
      bEdgeEnd = true;
      bDone = true;
      // very last one has a setting
    } else if (it2 == color_stops.end() - 1) {
      bDone = true;
    }

    // distribute offsets equally across range
    int ncolor_stops_t = std::distance(it, it2);
    if (bEdgeEnd)
      ncolor_stops_t--;

    if (ncolor_stops_t > 0) {
      double incr = 0;
      if (bEdgeEnd) {
        incr = (1 - it->offset) / ncolor_stops_t;
      } else {
        incr = (it2->offset - it->offset) / ncolor_stops_t;
        ncolor_stops_t--;
      }

      dOffset = it->offset;
      while (ncolor_stops_t) {
        it++;
        dOffset += incr;
        it->offset = dOffset;
        it->bAutoOffset = false;
        ncolor_stops_t--;
      }
    }
    // forward to next range
    it = it2;
  }
  // add the color stops
  std::for_each(color_stops.begin(), color_stops.end(), [=](auto &n) {
    if (n.bRGBA)
      cairo_pattern_add_color_stop_rgba(pattern, n.offset, n.r, n.g, n.b, n.a);
    else
      cairo_pattern_add_color_stop_rgb(pattern, n.offset, n.r, n.g, n.b);
  });
}

/**
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::color_stops_provider_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  for (auto n : color_stops)
    hash_combine(__value, n.hash_code());

  return __value;
}
