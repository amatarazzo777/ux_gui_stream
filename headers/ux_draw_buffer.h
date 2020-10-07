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
#pragma once

/**
@author Anthony Matarazzo
@file ux_draw_buffer.hpp
@date 9/19/20
@version 1.0
@brief off screen cairo complete buffer

*/

namespace uxdevice {

/**
 * @internal
 * @enum draw_buffer_format_t
 * @brief
*/
enum class draw_buffer_format_t {
  none,
  svg,
  png,
  inline_png,
  inline_svg,

};

/**
 * @class draw_buffer_t
 * @brief offscreen buffer wrapping cairo
*/
class draw_buffer_t : public abstract_emit_cr_a_t<order_render>,
                      virtual public system_error_t,
                      virtual hash_members_t {
public:
  draw_buffer_t() {}

  draw_buffer_t(double _width, double _height)
      : width(_width), height(_height) {
    rendered = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int)width,
                                          (int)height);
  }

  operator bool() const { return rendered != nullptr; }

  draw_buffer_t(int _width, int _height)
      : width((double)_width), height((double)_height) {
    rendered = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, _width, _height);
    cr = cairo_create(rendered);
  }

  draw_buffer_t(std::string &description, double _width, int _height)
      : width(_width), height(_height) {
    read_image(description, width, height);
  }
  draw_buffer_t(std::string &description, int _width, int _height)
      : width((double)_width), height((double)_height) {
    read_image(description, width, height);
  }

  /// @brief move constructor
  draw_buffer_t(draw_buffer_t &&other) noexcept
      : cr(other.cr), rendered(other.rendered), format(other.format) {}

  /// @brief  copy constructor
  draw_buffer_t(const draw_buffer_t &other)
      : cr(cairo_reference(other.cr)),
        rendered(cairo_surface_reference(other.rendered)),
        format(other.format) {}

  /// @brief copy assignment
  draw_buffer_t &operator=(const draw_buffer_t &other) {
    cr = cairo_reference(other.cr);
    rendered = cairo_surface_reference(other.rendered);
    format = other.format;
    return *this;
  }

  /// @brief move assignment
  draw_buffer_t &operator=(const draw_buffer_t &&other) noexcept {
    cr = other.cr;
    rendered = other.rendered;
    format = other.format;
    return *this;
  }

  ~draw_buffer_t() {
    if (cr)
      cairo_destroy(cr);
    if (rendered)
      cairo_surface_destroy(rendered);
  }

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(draw_buffer_t)), cr, rendered,
                 format, width, height);
    return __value;
  }

  void emit(cairo_t *cr);
  void emit(cairo_t *cr, coordinate_t *a);
  void flush(void) { cairo_surface_flush(rendered); }
  bool is_valid(void) { return rendered != nullptr; }

  cairo_t *cr = {};
  cairo_surface_t *rendered = {};
  draw_buffer_format_t format = {};
  double width = {};
  double height = {};

public:
  void read_image(std::string &data, const double w, const double h);

private:
  cairo_status_t read_contents(const gchar *file_name, guint8 **contents,
                               gsize *length);

  void image_surface_SVG(const bool bDataPassed, std::string &info,
                         const double width, const double height);

#if defined(USE_STACKBLUR)
  /// Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>
  /// Stackblur algorithm by Mario Klingemann
  /// Details here:
  /// http://www.quasimondo.com/StackBlurForCanvas/StackBlurDemo.html
  /// C++ implemenation base from:
  /// https://gist.github.com/benjamin9999/3809142
  /// http://www.antigrain.com/__code/include/agg_blur.h.html
  /// This version works only with RGBA color
public:
  void blur_image(const unsigned int radius);

#elif defined(USE_SVGREN)
  // box blur by Ivan Gagis <igagis@gmail.com>
  // svgren project.
public:
  void blur_image(unsigned int radius);

private:
  cairo_surface_t *blur_image(const unsigned int radius);
  void box_blur_horizontal(std::uint8_t *dst, const std::uint8_t *src,
                           unsigned dstStride, unsigned srcStride,
                           unsigned width, unsigned height, unsigned boxSize,
                           unsigned boxOffset, unsigned channel);

  void box_blur_vertical(std::uint8_t *dst, const std::uint8_t *src,
                         unsigned dstStride, unsigned srcStride, unsigned width,
                         unsigned height, unsigned boxSize, unsigned boxOffset,
                         unsigned channel);

  cairo_surface_t *cairo_image_surface_blur(std::array<double, 2> stdDeviation);

#endif // defined

}; // namespace uxdevice

} // namespace uxdevice
