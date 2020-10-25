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
 * @file data.cpp
 * @date 10/23/20
 * @version 1.0
 * @brief emit implementations for objects.
 */
// clang-format off

#include <api/text/data.h>
#include <base/unit_object.h>

// clang-format on

/**
 * @internal
 * @fn text_data_t::emit
 * @param PangoLayout *layout
 * @brief sets that text of the layout to the one stored withi nthe text_data_t
 * object. The text_data_t object contains a text_data_storage_t which is a
 * std::variant holding the information within a string, string_view or
 */
void uxdevice::text_data_t::emit(PangoLayout *layout) {
  std::string_view sinternal = std::string_view(pango_layout_get_text(layout));
  auto text_data_visitor =
      overload_visitors_t{[&](std::string &s) {
                            if (s.compare(sinternal) != 0)
                              pango_layout_set_text(layout, s.data(), -1);
                          },

                          [&](std::string_view &s) {
                            if (s.compare(sinternal) != 0)
                              pango_layout_set_text(layout, s.data(), -1);
                          },

                          [&](std::shared_ptr<std::string> ps) {
                            if (ps->compare(sinternal) != 0)
                              pango_layout_set_text(layout, ps->data(), -1);
                          },

                          [&](std::shared_ptr<std::string_view> ps) {
                            if (ps->compare(sinternal) != 0)
                              pango_layout_set_text(layout, ps->data(), -1);
                          },

                          [&](std::shared_ptr<std::stringstream> ps) {
                            std::string stmp = ps->str();
                            if (stmp.compare(sinternal) != 0)
                              pango_layout_set_text(layout, stmp.data(), -1);
                          }};

  std::visit(text_data_visitor, value);
}

/**
 * @internal
 * @fn text_data_t::hash_code
 * @brief
 * @return std::size_t - the hashed value
 */
std::size_t uxdevice::text_data_t::hash_code(void) const noexcept {
  std::size_t __value = std::type_index(typeid(text_data_t)).hash_code();

  auto text_data_visitor = overload_visitors_t{
      [&](std::string s) { hash_combine(__value, s); },
      [&](std::string_view s) { hash_combine(__value, s); },
      [&](std::shared_ptr<std::string> ps) { hash_combine(__value, *ps); },
      [&](std::shared_ptr<std::string_view> ps) { hash_combine(__value, *ps); },
      [&](std::shared_ptr<std::stringstream> ps) {
        std::string stmp = ps->str();
        hash_combine(__value, stmp);
      }};

  std::visit(text_data_visitor, value);

  return __value;
}
