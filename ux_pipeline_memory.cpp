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

/**
\author Anthony Matarazzo
\file uxunitmemory.cpp
\date 9/7/20
\version 1.0
\brief
*/
#include "ux_device.hpp"

/**
  \fn unit_memory_linkages

*/
template <typename ATTR>
void uxdevice::pipeline_memory_t<ATTR>::unit_memory_linkages(
    display_context_t &other) {
  for (auto n : other.storage)
    if (n.first == std::type_index(typeid(ATTR)))
      storage[n.first] = n.second;
}
