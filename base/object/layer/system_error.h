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
 * @file system_error.h
 * @date 9/12/20
 * @version 1.0
 * @details
 */

namespace uxdevice {


/**
 * @class system_error_t
 * @brief error handling object for the system level report.
 */
class system_error_t {
public:
  system_error_t();
  virtual ~system_error_t();
  /// @brief copy constructor
  system_error_t(const system_error_t &other);

  /// @move constructor
  system_error_t(system_error_t &&other);

  /// @copy assignment operator
  system_error_t &operator=(const system_error_t &other);
  /// @move assignment operator
  system_error_t &operator=(const system_error_t &&other) noexcept;
  /**
   * @internal
   * @fn error_report
   * @overload
   * @brief provides error code as a string which is useful when errors occur
   * that need textual description.
   */
  void error_report(std::string sfile, const std::size_t ln, std::string sfunc,
                    std::string edescription);

  /**
   *
   */
  void throw_error_report(std::string sfile, const std::size_t ln,
                          std::string sfunc, std::string edescription);

  /**
   * @internal
   * @fn error_report
   * @overload
   * @brief provides error code as a string which is useful when errors occur
   * that need textual description.
   */
  void error_report(std::string_view sfile, const std::size_t ln,
                    std::string_view sfunc, std::string_view edescription);

  /**
   * @internal
   * @fn error_report
   * @overload
   * @brief provides error code as a string which is useful when errors occur
   * that need textual description.
   */
  void error_report(const std::string_view &description);

  /**
   * @internal
   * @fn error_report
   * @overload
   * @brief provides error code as a string which is useful when errors occur
   * that need textual description.
   */
  void error_report(const std::string_view &sfunc, const std::size_t linenum,
                    const std::string_view &sfile, const cairo_status_t stat);

  /**
   * @internal
   * @fn error_check
   * @overload
   * @brief
   */
  bool error_check(void);

  /**
   * @internal
   * @fn error_check
   * @overload
   * @brief
   */
  bool error_check(cairo_surface_t *sur);

  /**
   * @internal
   * @fn error_check
   * @overload
   * @brief
   */
  bool error_check(cairo_t *cr);

  /**
   * @internal
   * @fn error_text
   * @brief
   */
  std::string error_text(void);

  /**
   * @internal
   * @fn error_clear
   * @brief
   */
  void error_clear(void);

  std::atomic_flag lockErrors = ATOMIC_FLAG_INIT;
  std::list<std::string> _errors = {};
};
} // namespace uxdevice
