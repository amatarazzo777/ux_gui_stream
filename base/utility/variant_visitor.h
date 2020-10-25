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
 * @file variant_visitor.hpp
 * @date 9/7/20
 * @version 1.0
 * @brief
 */

/**
 * @internal
 * @struct hash_members_t
 * @tparam ... - variadic parameter expanding to each listed visitor lambda.
 * @brief Creates structure for std::visit pattern.
 * From:
 * https://stackoverflow.com/questions/52088928/trying-to-return-the-value-from-stdvariant-using-stdvisit-and-a-lambda-expre
 */
namespace uxdevice {
template <class... Fs> struct overload_visitors_t : Fs... {
  using Fs::operator()...;
};
template <class... Fs> overload_visitors_t(Fs...) -> overload_visitors_t<Fs...>;
} // namespace uxdevice
