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
 * @file ux_error.h
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
  system_error_t() {}
  system_error_t(const system_error_t &other) {}

  virtual ~system_error_t() {}
#define UX_ERRORS_SPIN                                                         \
  while (lockErrors.test_and_set(std::memory_order_acquire))

#define UX_ERRORS_CLEAR lockErrors.clear(std::memory_order_release);

  /**
   * @internal
   * @fn error_report
   * @overload
   * @brief provides error code as a string which is useful when errors occur
   * that need textual description.
   */
  void error_report(std::string sfile, const std::size_t ln, std::string sfunc,
                    std::string edescription) {
    UX_ERRORS_SPIN;
    std::stringstream ss;
    ss << sfile << "\n"
       << sfunc << "(" << ln << ") -  " << sfunc << "() :-->\n"
       << edescription << "\n";
    _errors.emplace_back(ss.str());

    UX_ERRORS_CLEAR;
  }
  /**
   * @internal
   * @fn error_report
   * @overload
   * @brief provides error code as a string which is useful when errors occur
   * that need textual description.
   */
  void error_report(std::string_view sfile, const std::size_t ln,
                    std::string_view sfunc, std::string_view edescription) {
    UX_ERRORS_SPIN;
    std::stringstream ss;
    ss << sfile << "\n"
       << sfunc << "(" << ln << ") -  " << sfunc << "() :-->\n"
       << edescription << "\n";
    _errors.emplace_back(ss.str());

    UX_ERRORS_CLEAR;
  }

  /**
   * @internal
   * @fn error_report
   * @overload
   * @brief provides error code as a string which is useful when errors occur
   * that need textual description.
   */
  void error_report(const std::string_view &description) {
    _errors.emplace_back(description);
  }

  /**
   * @internal
   * @fn error_report
   * @overload
   * @brief provides error code as a string which is useful when errors occur
   * that need textual description.
   */
  void error_report(const std::string_view &sfunc, const std::size_t linenum,
                    const std::string_view &sfile, const cairo_status_t stat) {
    error_report(sfunc, linenum, sfile,
                 std::string_view(cairo_status_to_string(stat)));
  }

  /**
   * @internal
   * @fn error_check
   * @overload
   * @brief
   */
  bool error_check(void) { return _errors.empty(); }

  /**
   * @internal
   * @fn error_check
   * @overload
   * @brief
   */
  bool error_check(cairo_surface_t *sur) {
    auto stat = cairo_surface_status(sur);
    if (stat)
      error_report(__func__, __LINE__, __FILE__, stat);

    return _errors.empty();
  }

  /**
   * @internal
   * @fn error_check
   * @overload
   * @brief
   */
  bool error_check(cairo_t *cr) {
    auto stat = cairo_status(cr);
    if (stat) {
      error_report(__func__, __LINE__, __FILE__, stat);
      return true;
    }
    return false;
  }

  /**
   * @internal
   * @fn error_text
   * @brief
   */
  std::string error_text(void) {
    UX_ERRORS_SPIN;
    std::string ret;
    for (auto s : _errors)
      ret += s + "\n";

    UX_ERRORS_CLEAR;
    return ret;
  }

  /**
   * @internal
   * @fn error_clear
   * @brief
   */
  void error_clear(void) { _errors.clear(); }

  std::atomic_flag lockErrors = ATOMIC_FLAG_INIT;
  std::list<std::string> _errors = {};
};
} // namespace uxdevice
