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
 * @file client_interface.cpp
 * @date 10/24/20
 * @version 1.0
 * @brief OS implementation of loading a shared library. The four function
 *
 */
// clang-format off

#include <base/std_base.h>
#include <dlfdn.h>

#include <api/client_interface.h>
#include "client_interface.h"

// clang-format on

uxdevice::os_linux_client_interface_t::os_linux_client_interface_t() {

  /** @brief these functions are needed for the guid interface to work. The
   * details of the load are reading of the guid table is located in the base.
   * The derived os implementation of the client interface should initializes
   * the function members and invoke initialize. */
  fn_library_open = std::bind(
      static_cast<void (os_linux_client_interface_t::*)(const std::string &)>(
          &os_linux_client_interface_t::library_open),
      std::placeholders::_1);

  fn_library_close =
      std::bind(static_cast<void (os_linux_client_interface_t::*)(void)>(
          &os_linux_client_interface_t::library_close));

  fn_library_symbol = std::bind(
      static_cast<void (os_linux_client_interface_t::*)(const std::string &)>(
          &os_linux_client_interface_t::library_symbol),
      cl, std::placeholders::_1);

  fn_library_error =
      std::bind(static_cast<std::string (os_linux_client_interface_t::*)(void)>(
                    &os_linux_client_interface_t::library_error),
                cl, std::placeholders::_1);
}

/**
 * @internal
 * @fn library_open
 * @brief The function issues the dlopen and stores the handle in dl_ptr.
 *
 */
uxdevice::os_linux_client_interface_t::library_open(
    std::string &_library_name) {

  // build correct path from os indenpendant name. (.so vs .dl). This would also
  // be a place to add current path and a software search path.
  library_name = _library_name;
  library_filename = "./" + sym + ".so";
  dl_handle = dlopen(library_filename.data(), RTLD_LAZY);

  if (!dl_handle) {
    std::stringstream serror;
    serror << "Cannot load library: " << dlerror() << "  " << library_name
           << " given, searched for  " << library_filename;
    error_message = serror.str();
    return;
  }
}

/**
 * @internal
 * @fn library_close
 * @brief The function issues the dlopen and stores the handle in dl_ptr.
 *
 */
uxdevice::os_linux_client_interface_t::library_close(
    std::string &_library_name) {

  if (dl_handle)
    if (dlclose(dl_handle)) {
      std::stringstream serror;
      serror << "Error close library: " << dlerror() << "  " << library_name
             << " given, searched for  " << library_filename;
      error_message = serror.str();
    }

  // clear it.
  dl_handle = nullptr;
}

/**
 * @internal
 * @fn library_symbol
 * @brief loads a symbol, low level pointer void * used with bind.
 *
 */
void *
uxdevice::os_linux_client_interface_t::library_symbol(std::string &symbol) {
  void *ptr_sym = {};

  if (dl_handle) {
    ptr_sym = dlsym(dl_handle, symbol.data());

    const char *dlsym_error = dlerror();
    if (dlsym_error) {
      std::stringstream serror;
      serror << "Cannot load library: " << dlsym_error << "  " << symbol
             << " given, searched for  " << sfilename;
      error_message = serror.str();
    }
  }

  return ptr_sym;
}

std::string uxdevice::os_linux_client_interface_t::library_error(void) {
  return error_message;
}
