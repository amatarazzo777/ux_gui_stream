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
@file negative_arc_storage.h
@date 10/23/20
@version 1.0
@brief
*/

namespace uxdevice {

/**
 @internal
 @class negative_arc_storage_t
 @brief

 @details


 */
class negative_arc_storage_t : virtual public hash_members_t {
public:
  negative_arc_storage_t();
  negative_arc_storage_t(double _xc, double _yc, double _radius, double _angle1,
                         double _angle2);

  virtual ~negative_arc_storage_t();

  std::size_t hash_code(void) const noexcept;

  double xc = {};
  double yc = {};
  double radius = {};
  double angle1 = {};
  double angle2 = {};
};
} // namespace uxdevice
UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::negative_arc_storage_t)
