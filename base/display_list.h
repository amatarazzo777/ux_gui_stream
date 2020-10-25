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
 * @file display_list.h
 * @date 10/22/20
 * @version 1.0
 * @brief display list using mutex for storage
 */

#pragma once

/**
 * @namespace uxdevice
 * @brief
 */

/**
 * @typedef display_unit_list_t
 * @brief type that holds the list that is input directly from the user. The
 * sequence is as it is input.
 */
class display_unit_t;
typedef std::list<std::shared_ptr<display_unit_t>> display_unit_list_t;

/**
 * @class display_list
 *
 */
struct display_list_t {
  /**
   * @var  display_list_storage
   * @brief holds the object units information as a display unit base. The base
   * may be casted to other object types using the std::dynamic_pointer_cast
   * function template. However, interface objects that use the multithreading
   * mutex types should be used. see display_list<T>
   *
   */
  display_unit_list_t display_list_storage = {};
  display_unit_list_t::iterator itDL_Processed = display_list_storage.begin();

  // interface between client and API rendering threads.
  std::mutex display_list_mutex = {};

  /**
   * @fn display_list
   * @brief template function to insert into the display list and perform
   * initialization based upon the type. The c++ constexpr conditional compiling
   * functionality is used to trim the run time and code size. One accepts a
   * constant double reference object in a parampack/
   */
  template <class T, typename... Args>
  std::shared_ptr<T> display_list(const Args &... args) {
    return display_list<T>(std::make_shared<T>(args...));
  }

  template <class T, typename... Args>
  std::shared_ptr<T> display_list(const Args &&... args) {
    return display_list<T>(std::make_shared<T>(args...));
  }

  /**
   * @fn display_list
   * @brief
   */
  template <class T>
  std::shared_ptr<T> display_list(const std::shared_ptr<T> ptr) {
    std::lock_guard<std::mutex> lock(display_list_mutex);
    display_list_storage.emplace_back(ptr);
    return ptr;
  }

  /**
   * @fn display_list_clear
   * @brief
   */
  void display_list_clear(void) {
    std::lock_guard<std::mutex> lock(display_list_mutex);
    display_list_storage.clear();
  }
};
}
