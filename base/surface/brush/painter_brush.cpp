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
 * @file painter_brush.cpp
 * @date 5/12/20
 * @version 1.0
 * @details Class provides the painting object interface which invokes the
 * appropriate cairo API. The notable virtual method is the emit function which
 * applies the cairo text_color_t setting.
 */
// clang-format off

#include <base/unit_object.h>
#include "painter_brush.h"

// clang-format on

/**
 * @internal
 * @fn
 * @brief color given as a uint32 value
 */
uxdevice::painter_brush_t::painter_brush_t(u_int32_t c) {

  auto r = static_cast<u_int8_t>(c >> 16) / 255.0;
  auto g = static_cast<u_int8_t>(c >> 8) / 255.0;
  auto b = static_cast<u_int8_t>(c) / 255.0;
  auto a = 1.0;

  data_storage = color_definition_t{"u_int32_t RGB", r, g, b, a};
}

/**
 * @fn painter_brush_t operator =&(const painter_brush_t&)
 * @brief copy assignment
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t &
uxdevice::painter_brush_t::operator=(const painter_brush_t &other) {
  matrix_t::operator=(other);
  data_storage = other.data_storage;
  return *this;
}

/**
 * @fn painter_brush_t operator =&(painter_brush_t&&)
 * @brief move assignment
 *
 * @param other
 * @return
 */
uxdevice::painter_brush_t &
uxdevice::painter_brush_t::operator=(painter_brush_t &&other) noexcept {
  data_storage = std::move(other.data_storage);
  matrix_t::operator=(other);
  return *this;
}

/**
 * @internal
 * @brief setup a std::list of paint type creation functions, factories that
 * check if the given string is processed by an object. The creation order
 * is in the same order as it appears below. This list is initialized once at
 * application start. Each of the object constructors is expected to parse the
 * string. If it is applicable, the is_loaded flag is set to true. Otherwise the
 * factory lambda returns std::monostate.
 */
uxdevice::painter_brush_t::paint_factories_t
    uxdevice::painter_brush_t::paint_factories = {

        /**
         * @internal
         * @brief factory lambda for image_block_pattern_source_definition_t
         * given a string. */
        [](auto s) {
          data_storage_t o_ret = {};
          auto o = image_block_pattern_source_definition_t(
              s, extend_options_t::repeat, filter_options_t::fast);
          if (o.is_loaded)
            o_ret = o;

          return o_ret;
        },

        /**
         * @internal
         * @brief factory lambda for linear_gradient_definition_t given a
         * string. */
        [](auto s) {
          data_storage_t o_ret = {};
          auto o = linear_gradient_definition_t(s);
          if (o.is_loaded)
            o_ret = o;

          return o_ret;
        },

        /**
         * @internal
         * @brief factory lambda for radial_gradient_definition_t given a
         * string. */
        [](auto s) {
          data_storage_t o_ret = {};
          auto o = radial_gradient_definition_t(s);
          if (o.is_loaded)
            o_ret = o;

          return o_ret;
        },

        /**
         * @internal
         * @brief factory lambda for color_definition_t given a string. */
        [](auto s) {
          data_storage_t o_ret = {};
          auto o = color_definition_t(s);
          if (o.is_loaded)
            o_ret = o;

          return o_ret;
        }};

/**
 * @internal
 * @fn create
 * @brief The routine handles the creation of the pattern or surface. Patterns
 * can be an image_block_t file, a description of a linear, actual parameters of
 * linear, a description of a radial, the actual radial parameters stored. SVG
 * inline or a base64 data set.
 */
bool uxdevice::painter_brush_t::create(void) {

  // setup visitor function to get member variable "is_processed"
  auto fn_visit_is_processed = [&]() -> bool {
    return std::visit(
        [&](auto &&__v) -> bool {
          using T = std::decay_t<decltype(__v)>;
          if constexpr (std::is_same_v<T, std::monostate>)
            return false;
          else
            return __v.is_processed;
        },
        data_storage);
  };

  // already created?
  if (fn_visit_is_processed())
    return true;

  bool object_created = false;

  // if a description was provided, determine how it should be interpreted
  if (std::holds_alternative<descriptive_definition_t>(data_storage)) {

    auto ddt = std::get<descriptive_definition_t>(data_storage);

    /** @brief iterate the paint types to see which one is applicable. when one
     * is successfully created, the data_storage will no longer hold the
     * std::monostate */
    for (auto n : paint_factories) {

      // try to create
      data_storage = n(ddt.description);

      // success?
      if (!std::holds_alternative<std::monostate>(data_storage)) {
        object_created = true;
        break;
      }
    }
  }

  return object_created;
}

/**
 * @internal
 * @fn emit
 * @param cairo_t *cr
 * @brief The paint is loaded if need be. The paint is set for the passed
 * surface.
 */
void uxdevice::painter_brush_t::emit(cairo_t *cr) {
  std::visit(
      [&](auto &&o) {
        using T = std::decay_t<decltype(o)>;
        if constexpr (std::is_same_v<T, std::monostate> ||
                      std::is_same_v<T, descriptive_definition_t>)
          return;
        else {
          if (!o.is_processed)
            create();
          if (o.is_processed)
            o.emit(cr);
        }
      },
      data_storage);
}

/**
 * @internal
 * @fn emit
 * @param cairo_t *cr
 * @param coordinate_t *a
 * @brief The paint is loaded if need be. the paint is set for the passed
 * surface.
 */
void uxdevice::painter_brush_t::emit(cairo_t *cr, coordinate_t *a) {

  std::visit(
      [&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate> ||
                      std::is_same_v<T, descriptive_definition_t> ||
                      std::is_same_v<T, color_definition_t>)
          return;
        else {
          if (!arg.is_processed)
            create();
          if (arg.is_processed) {
            translate(-a->x, -a->y);
            arg.emit(cr, a);
          }
        }
      },
      data_storage);
}

/**
 * @internal
 * @fn std::size_t hash_code(void)const
 * @brief
 *
 * @return
 */
std::size_t uxdevice::painter_brush_t::hash_code(void) const noexcept {
  std::size_t __value = {};
  hash_combine(__value, std::type_index(typeid(this)));

  hash_combine(__value, std::visit(
                            [](auto &&arg) -> std::size_t {
                              using T = std::decay_t<decltype(arg)>;
                              if constexpr (std::is_same_v<T, std::monostate>)
                                return std::type_index(typeid(T)).hash_code();
                              else
                                return arg.hash_code();
                            },
                            data_storage));

  return __value;
}
