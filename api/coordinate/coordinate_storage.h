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
@author Anthony Matarazzo
@file coordinate_storage.h
@date 10/23/20
@version 1.0
@brief
*/

#pragma once

namespace uxdevice {
class hash_members_t;
class display_context_t;

/**

 @class coordinate_storage_t
 @brief storage class used by
 the coordinate_t object.

 @details The constructor interface is inherited by objects that are display
 units. This object holds the storage. Any translations of user defined literals
 may take place here such as calculating the percentages. However, this
 functionality is not incorporated at this time.

 */

class coordinate_storage_t : virtual public hash_members_t {
public:
  coordinate_storage_t();
  coordinate_storage_t(double _x, double _y, double _w, double _h);
  coordinate_storage_t(double _x, double _y);
  virtual ~coordinate_storage_t();

  std::size_t hash_code(void) const noexcept;

  double x = {};
  double y = {};
  double w = {};
  double h = {};
};
} // namespace uxdevice

UX_REGISTER_STD_HASH_SPECIALIZATION(uxdevice::coordinate_storage_t)
