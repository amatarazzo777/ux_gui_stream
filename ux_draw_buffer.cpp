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
 * @file ux_draw_buffer.cpp
 * @date 9/19/20
 * @version 1.0
 * @brief
 *   References to source and functions that are provided by others.
 *
 *   ===============
 *   "box blur" by Ivan Gagis <igagis@gmail.com>
 *   Implementation adopted from the svgren project.
 *
 *   NOTE: see https://www.w3.org/TR/SVG/filters.html#feGaussianBlurElement
 *    for Gaussian Blur approximation algorithm.
 *
 *   ================
 *   Code spinet from base64 decode snippet in c++
 *   stackoverflow.com/base64 decode snippet in c++ - Stack Overflow.html
 *
 *   ================
 *   Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>
 *   Stackblur algorithm by Mario Klingemann
 *
 *   Details here:
 *    http://www.quasimondo.com/StackBlurForCanvas/StackBlurDemo.html
 *
 *   C++ implementation base from:
 *    https://gist.github.com/benjamin9999/3809142
 *    http://www.antigrain.com/__code/include/agg_blur.h.html
 *
 *   This version works only with RGBA color
 */
#include <ux_compile_options.h>
#include <ux_base.h>
#include <ux_system_error.h>
#include <ux_visitor_interface.h>
#include <ux_hash.h>

#include <ux_enums.h>

#include <ux_matrix.h>
#include <ux_draw_buffer.h>
#include <ux_painter_brush.h>

#include <ux_pipeline_memory.h>

#include <ux_display_visual.h>
#include <ux_display_context.h>
#include <ux_display_unit_base.h>

#include <ux_coordinate.h>



using namespace std;
using namespace uxdevice;

/**
 * @internal
 * @fn emit
 * @param cairo_t *tocr
 * @brief output surface to the requested surface.
 */
void uxdevice::draw_buffer_t::emit(cairo_t *tocr) {
  cairo_set_source_surface(tocr, rendered, 0, 0);
  cairo_rectangle(tocr, 0, 0, width, height);
  cairo_fill(tocr);
}

/**
 * @internal
 * @fn emit
 * @param cairo_t *tocr
 * @param coordinate_t &a
 * @brief output surface to the requested surface.
 */
void uxdevice::draw_buffer_t::emit(cairo_t *tocr, coordinate_t *a) {
  cairo_set_source_surface(tocr, rendered, a->x, a->y);
  cairo_rectangle(tocr, a->x, a->y, a->w, a->h);
  cairo_fill(tocr);
}

/**
 * @internal
 * @fn read_contents
 * @param const gchar *file_name
 * @param guint8 **contents - g_new
 * @param gsize *length
 * @brief reads the file based upon the name. Allocates a buffer "**contents" as
 * a double pointer.
 */
cairo_status_t uxdevice::draw_buffer_t::read_contents(const gchar *file_name,
                                                      guint8 **contents,
                                                      gsize *length) {
  GFile *file;
  GFileInputStream *input_stream;
  gboolean success = FALSE;
  cairo_status_t status = CAIRO_STATUS_SUCCESS;

  file = g_file_new_for_commandline_arg(file_name);
  input_stream = g_file_read(file, NULL, NULL);
  if (input_stream) {
    GFileInfo *file_info;

    file_info = g_file_input_stream_query_info(
      input_stream, G_FILE_ATTRIBUTE_STANDARD_SIZE, NULL, NULL);
    if (file_info) {
      gsize bytes_read;

      *length = g_file_info_get_size(file_info);
      *contents = g_new(guint8, *length);
      success = g_input_stream_read_all(G_INPUT_STREAM(input_stream), *contents,
                                        *length, &bytes_read, NULL, NULL);
      if (!success)
        status = CAIRO_STATUS_READ_ERROR;

      g_object_unref(file_info);
    } else {
      status = CAIRO_STATUS_READ_ERROR;
    }
    g_object_unref(input_stream);
  } else {
    status = CAIRO_STATUS_FILE_NOT_FOUND;
  }

  g_object_unref(file);

  return status;
}

/**
 * @internal
 * @fn image_surface_SVG
 * @param bool bDataPassed
 * @param std::string &info
 * @param double _width
 * @param double _height
 * @brief creates a drawing_buffer_t surface from an svg using the rsvg library.
 * Data can be passed in as a string inlined svg that perhaps would be
 * parameterized by the caller. Or as a filename.
 */
void uxdevice::draw_buffer_t::image_surface_SVG(const bool bDataPassed,
                                                std::string &info,
                                                const double _width,
                                                const double _height) {

  guint8 *contents = nullptr;
  gsize length = 0;
  RsvgHandle *handle = nullptr;
  RsvgDimensionData dimensions;
  cairo_status_t status = CAIRO_STATUS_SUCCESS;
  double dWidth = 0;
  double dHeight = 0;

  // set member variables
  width = _width;
  height = _height;

  // if data was passed as a string, set the contents pointer to it
  // as interface.
  if (bDataPassed) {
    contents = reinterpret_cast<guint8 *>(info.data());
    length = info.size();
  } else {
    // read the file. the contents and the length are allocated by the caller.
    // the info at this point has the file name.
    status = read_contents(info.data(), &contents, &length);
    if (status != CAIRO_STATUS_SUCCESS) {
      goto error_exit;
    }
  }
  // create a rsvg handle
  handle = rsvg_handle_new_from_data(contents, length, NULL);
  if (!handle) {
    g_free(contents);
    status = CAIRO_STATUS_READ_ERROR;
    goto error_exit;
  }

  // scale to the  requested.
  dWidth = width;
  dHeight = height;
  rsvg_handle_get_dimensions(handle, &dimensions);
  if (dWidth < 1) {
    dWidth = dimensions.width;
  } else {
    dWidth /= dimensions.width;
  }

  if (dHeight < 1) {
    dHeight = dimensions.height;
  } else {
    dHeight /= dimensions.height;
  }

  // create image surface to hold image
  rendered = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  status = cairo_surface_status(rendered);
  if (status != CAIRO_STATUS_SUCCESS) {
    goto error_exit;
  }

  cr = cairo_create(rendered);
  status = cairo_status(cr);
  if (status != CAIRO_STATUS_SUCCESS) {
    goto error_exit;
  }

  cairo_scale(cr, dWidth, dHeight);
  status = cairo_status(cr);
  if (status != CAIRO_STATUS_SUCCESS) {
    goto error_exit;
  }

  // render the svg to cairo
  if (!rsvg_handle_render_cairo(handle, cr)) {
    status = CAIRO_STATUS_READ_ERROR;
    goto error_exit;
  }

  // clean up
  cairo_destroy(cr);
  cr = nullptr;
  if (!bDataPassed)
    g_free(contents);

  g_object_unref(handle);

  return;

error_exit:
  if (cr) {
    cairo_destroy(cr);
    cr = nullptr;
  }
  if (rendered) {
    cairo_surface_destroy(rendered);
    rendered = nullptr;
  }
  if (!bDataPassed && contents)
    g_free(contents);

  if (handle)
    g_object_unref(handle);

  return;
}

/**
 * @internal
 * @fn read_image
 * @param const std::string &data
 * @param const double w
 * @param const double h
 * @brief reads the image, which can be described in multiple formats such as
 * supported by the system. This is the main input routine.
 */
void uxdevice::draw_buffer_t::read_image(std::string &data, const double w,
                                         const double h) {
  const string dataPNG = string("data:image/png;base64,");
  const string dataSVG = string("<?xml");

  if (data.size() == 0) {
    rendered = nullptr;
    return;
  }

  // data is passed as base 64 PNG?
  if (data.compare(0, dataPNG.size(), dataPNG) == 0) {

    // from stackoverflow snippet
    typedef struct _readInfo {
      unsigned char *data = nullptr;
      size_t dataLen = 0;
      int val = 0;
      int valB = -8;
      size_t decodePos = 0;
    } readInfo;

    readInfo pngData;

    pngData.dataLen = data.size();
    pngData.data = reinterpret_cast<unsigned char *>(data.data());
    pngData.val = 0;
    pngData.valB = -8;
    pngData.decodePos = dataPNG.size();

    cairo_read_func_t fn = [](void *closure, unsigned char *data,
                              unsigned int length) -> cairo_status_t {
      static const uint8_t lookup[] = {
        62,  255, 62,  255, 63,  52,  53, 54, 55, 56, 57, 58, 59, 60, 61, 255,
        255, 0,   255, 255, 255, 255, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
        10,  11,  12,  13,  14,  15,  16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
        255, 255, 255, 255, 63,  255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
        36,  37,  38,  39,  40,  41,  42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
      static_assert(sizeof(lookup) == 'z' - '+' + 1);
      readInfo *p = reinterpret_cast<readInfo *>(closure);

      size_t bytesDecoded = 0;
      while (bytesDecoded < length) {

        // requesting more than the size?
        if (p->decodePos > p->dataLen)
          return CAIRO_STATUS_READ_ERROR;

        uint8_t c = p->data[p->decodePos];

        if (c < '+' || c > 'z')
          return CAIRO_STATUS_READ_ERROR;

        c -= '+';
        if (lookup[c] >= 64)
          return CAIRO_STATUS_READ_ERROR;

        p->val = (p->val << 6) + lookup[c];
        p->valB += 6;
        if (p->valB >= 0) {
          *data = static_cast<unsigned char>((p->val >> p->valB) & 0xFF);
          data++;
          bytesDecoded++;
          p->valB -= 8;
        }

        p->decodePos++;
      }

      return CAIRO_STATUS_SUCCESS;
    };

    rendered = cairo_image_surface_create_from_png_stream(fn, &pngData);

    // if not successful read, set the contents to a null pointer.
    if (cairo_surface_status(rendered) != CAIRO_STATUS_SUCCESS)
      rendered = nullptr;

    // data in passed as a SVG text?
    // use w and h set by caller.
  } else if (data.compare(0, dataSVG.size(), dataSVG) == 0) {
    image_surface_SVG(true, data, w, h);

    // file name?
  } else if (data.find(".png") != std::string::npos) {
    rendered = cairo_image_surface_create_from_png(data.data());

    // if not successful read, set the contents to a null pointer.
    if (cairo_surface_status(rendered) != CAIRO_STATUS_SUCCESS)
      rendered = nullptr;

  } else if (data.find(".svg") != std::string::npos) {
    image_surface_SVG(false, data, w, h);
  }

  // if image was read report the size
  if (rendered) {
    width = cairo_image_surface_get_width(rendered);
    height = cairo_image_surface_get_height(rendered);
  }
}

#if defined(USE_STACKBLUR)
/**
 * @internal
 * @fn blur_image
 * @param unsigned int radius
 * @details
 * Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>
 * Stackblur algorithm by Mario Klingemann
 *
 * http://www.quasimondo.com/StackBlurForCanvas/StackBlurDemo.html
 * https://gist.github.com/benjamin9999/3809142
 * http://www.antigrain.com/__code/include/agg_blur.h.html
 *
 * This version works only with RGBA color
 *
 */
void uxdevice::draw_buffer_t::blur_image(const unsigned int radius) {
  static unsigned short const stackblur_mul[255] = {
    512, 512, 456, 512, 328, 456, 335, 512, 405, 328, 271, 456, 388, 335, 292,
    512, 454, 405, 364, 328, 298, 271, 496, 456, 420, 388, 360, 335, 312, 292,
    273, 512, 482, 454, 428, 405, 383, 364, 345, 328, 312, 298, 284, 271, 259,
    496, 475, 456, 437, 420, 404, 388, 374, 360, 347, 335, 323, 312, 302, 292,
    282, 273, 265, 512, 497, 482, 468, 454, 441, 428, 417, 405, 394, 383, 373,
    364, 354, 345, 337, 328, 320, 312, 305, 298, 291, 284, 278, 271, 265, 259,
    507, 496, 485, 475, 465, 456, 446, 437, 428, 420, 412, 404, 396, 388, 381,
    374, 367, 360, 354, 347, 341, 335, 329, 323, 318, 312, 307, 302, 297, 292,
    287, 282, 278, 273, 269, 265, 261, 512, 505, 497, 489, 482, 475, 468, 461,
    454, 447, 441, 435, 428, 422, 417, 411, 405, 399, 394, 389, 383, 378, 373,
    368, 364, 359, 354, 350, 345, 341, 337, 332, 328, 324, 320, 316, 312, 309,
    305, 301, 298, 294, 291, 287, 284, 281, 278, 274, 271, 268, 265, 262, 259,
    257, 507, 501, 496, 491, 485, 480, 475, 470, 465, 460, 456, 451, 446, 442,
    437, 433, 428, 424, 420, 416, 412, 408, 404, 400, 396, 392, 388, 385, 381,
    377, 374, 370, 367, 363, 360, 357, 354, 350, 347, 344, 341, 338, 335, 332,
    329, 326, 323, 320, 318, 315, 312, 310, 307, 304, 302, 299, 297, 294, 292,
    289, 287, 285, 282, 280, 278, 275, 273, 271, 269, 267, 265, 263, 261, 259};

  static unsigned char const stackblur_shr[255] = {
    9,  11, 12, 13, 13, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17,
    17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 24, 24, 24, 24};

  if (radius > 254)
    return;
  if (radius < 2)
    return;

  cairo_surface_flush(rendered);

  unsigned char *src =
    reinterpret_cast<unsigned char *>(cairo_image_surface_get_data(rendered));
  unsigned int w = cairo_image_surface_get_width(rendered);
  unsigned int h = cairo_image_surface_get_height(rendered);
  unsigned int x, y, xp, yp, i;
  unsigned int sp;
  unsigned int stack_start;
  unsigned char *stack_ptr;

  unsigned char *src_ptr;
  unsigned char *dst_ptr;

  unsigned long sum_r;
  unsigned long sum_g;
  unsigned long sum_b;
  unsigned long sum_a;
  unsigned long sum_in_r;
  unsigned long sum_in_g;
  unsigned long sum_in_b;
  unsigned long sum_in_a;
  unsigned long sum_out_r;
  unsigned long sum_out_g;
  unsigned long sum_out_b;
  unsigned long sum_out_a;

  unsigned int wm = w - 1;
  unsigned int hm = h - 1;
  unsigned int w4 = cairo_image_surface_get_stride(rendered);
  unsigned int mul_sum = stackblur_mul[radius];
  unsigned char shr_sum = stackblur_shr[radius];

  unsigned int div = (radius * 2) + 1;
  unsigned char *stack = new unsigned char[div * 4];

  unsigned int minY = 0;
  unsigned int maxY = h;

  for (y = minY; y < maxY; y++) {
    sum_r = sum_g = sum_b = sum_a = sum_in_r = sum_in_g = sum_in_b = sum_in_a =
      sum_out_r = sum_out_g = sum_out_b = sum_out_a = 0;

    src_ptr = src + w4 * y; // start of line (0,y)

    for (i = 0; i <= radius; i++) {
      stack_ptr = &stack[4 * i];
      stack_ptr[0] = src_ptr[0];
      stack_ptr[1] = src_ptr[1];
      stack_ptr[2] = src_ptr[2];
      stack_ptr[3] = src_ptr[3];
      sum_r += src_ptr[0] * (i + 1);
      sum_g += src_ptr[1] * (i + 1);
      sum_b += src_ptr[2] * (i + 1);
      sum_a += src_ptr[3] * (i + 1);
      sum_out_r += src_ptr[0];
      sum_out_g += src_ptr[1];
      sum_out_b += src_ptr[2];
      sum_out_a += src_ptr[3];
    }

    for (i = 1; i <= radius; i++) {
      if (i <= wm)
        src_ptr += 4;
      stack_ptr = &stack[4 * (i + radius)];
      stack_ptr[0] = src_ptr[0];
      stack_ptr[1] = src_ptr[1];
      stack_ptr[2] = src_ptr[2];
      stack_ptr[3] = src_ptr[3];
      sum_r += src_ptr[0] * (radius + 1 - i);
      sum_g += src_ptr[1] * (radius + 1 - i);
      sum_b += src_ptr[2] * (radius + 1 - i);
      sum_a += src_ptr[3] * (radius + 1 - i);
      sum_in_r += src_ptr[0];
      sum_in_g += src_ptr[1];
      sum_in_b += src_ptr[2];
      sum_in_a += src_ptr[3];
    }

    sp = radius;
    xp = radius;
    if (xp > wm)
      xp = wm;
    src_ptr = src + 4 * (xp + y * w); //   img.pix_ptr(xp, y);
    dst_ptr = src + y * w4;           // img.pix_ptr(0, y);
    for (x = 0; x < w; x++) {
      dst_ptr[0] = (sum_r * mul_sum) >> shr_sum;
      dst_ptr[1] = (sum_g * mul_sum) >> shr_sum;
      dst_ptr[2] = (sum_b * mul_sum) >> shr_sum;
      dst_ptr[3] = (sum_a * mul_sum) >> shr_sum;
      dst_ptr += 4;

      sum_r -= sum_out_r;
      sum_g -= sum_out_g;
      sum_b -= sum_out_b;
      sum_a -= sum_out_a;

      stack_start = sp + div - radius;
      if (stack_start >= div)
        stack_start -= div;
      stack_ptr = &stack[4 * stack_start];

      sum_out_r -= stack_ptr[0];
      sum_out_g -= stack_ptr[1];
      sum_out_b -= stack_ptr[2];
      sum_out_a -= stack_ptr[3];

      if (xp < wm) {
        src_ptr += 4;
        ++xp;
      }

      stack_ptr[0] = src_ptr[0];
      stack_ptr[1] = src_ptr[1];
      stack_ptr[2] = src_ptr[2];
      stack_ptr[3] = src_ptr[3];

      sum_in_r += src_ptr[0];
      sum_in_g += src_ptr[1];
      sum_in_b += src_ptr[2];
      sum_in_a += src_ptr[3];
      sum_r += sum_in_r;
      sum_g += sum_in_g;
      sum_b += sum_in_b;
      sum_a += sum_in_a;

      ++sp;
      if (sp >= div)
        sp = 0;
      stack_ptr = &stack[sp * 4];

      sum_out_r += stack_ptr[0];
      sum_out_g += stack_ptr[1];
      sum_out_b += stack_ptr[2];
      sum_out_a += stack_ptr[3];
      sum_in_r -= stack_ptr[0];
      sum_in_g -= stack_ptr[1];
      sum_in_b -= stack_ptr[2];
      sum_in_a -= stack_ptr[3];
    }
  }

  unsigned int minX = 0;
  unsigned int maxX = w;

  for (x = minX; x < maxX; x++) {
    sum_r = sum_g = sum_b = sum_a = sum_in_r = sum_in_g = sum_in_b = sum_in_a =
      sum_out_r = sum_out_g = sum_out_b = sum_out_a = 0;

    src_ptr = src + 4 * x; // x,0
    for (i = 0; i <= radius; i++) {
      stack_ptr = &stack[i * 4];
      stack_ptr[0] = src_ptr[0];
      stack_ptr[1] = src_ptr[1];
      stack_ptr[2] = src_ptr[2];
      stack_ptr[3] = src_ptr[3];
      sum_r += src_ptr[0] * (i + 1);
      sum_g += src_ptr[1] * (i + 1);
      sum_b += src_ptr[2] * (i + 1);
      sum_a += src_ptr[3] * (i + 1);
      sum_out_r += src_ptr[0];
      sum_out_g += src_ptr[1];
      sum_out_b += src_ptr[2];
      sum_out_a += src_ptr[3];
    }
    for (i = 1; i <= radius; i++) {
      if (i <= hm)
        src_ptr += w4; // +stride

      stack_ptr = &stack[4 * (i + radius)];
      stack_ptr[0] = src_ptr[0];
      stack_ptr[1] = src_ptr[1];
      stack_ptr[2] = src_ptr[2];
      stack_ptr[3] = src_ptr[3];
      sum_r += src_ptr[0] * (radius + 1 - i);
      sum_g += src_ptr[1] * (radius + 1 - i);
      sum_b += src_ptr[2] * (radius + 1 - i);
      sum_a += src_ptr[3] * (radius + 1 - i);
      sum_in_r += src_ptr[0];
      sum_in_g += src_ptr[1];
      sum_in_b += src_ptr[2];
      sum_in_a += src_ptr[3];
    }

    sp = radius;
    yp = radius;
    if (yp > hm)
      yp = hm;
    src_ptr = src + 4 * (x + yp * w); // img.pix_ptr(x, yp);
    dst_ptr = src + 4 * x;            // img.pix_ptr(x, 0);
    for (y = 0; y < h; y++) {
      dst_ptr[0] = (sum_r * mul_sum) >> shr_sum;
      dst_ptr[1] = (sum_g * mul_sum) >> shr_sum;
      dst_ptr[2] = (sum_b * mul_sum) >> shr_sum;
      dst_ptr[3] = (sum_a * mul_sum) >> shr_sum;
      dst_ptr += w4;

      sum_r -= sum_out_r;
      sum_g -= sum_out_g;
      sum_b -= sum_out_b;
      sum_a -= sum_out_a;

      stack_start = sp + div - radius;
      if (stack_start >= div)
        stack_start -= div;
      stack_ptr = &stack[4 * stack_start];

      sum_out_r -= stack_ptr[0];
      sum_out_g -= stack_ptr[1];
      sum_out_b -= stack_ptr[2];
      sum_out_a -= stack_ptr[3];

      if (yp < hm) {
        src_ptr += w4; // stride
        ++yp;
      }

      stack_ptr[0] = src_ptr[0];
      stack_ptr[1] = src_ptr[1];
      stack_ptr[2] = src_ptr[2];
      stack_ptr[3] = src_ptr[3];

      sum_in_r += src_ptr[0];
      sum_in_g += src_ptr[1];
      sum_in_b += src_ptr[2];
      sum_in_a += src_ptr[3];
      sum_r += sum_in_r;
      sum_g += sum_in_g;
      sum_b += sum_in_b;
      sum_a += sum_in_a;

      ++sp;
      if (sp >= div)
        sp = 0;
      stack_ptr = &stack[sp * 4];

      sum_out_r += stack_ptr[0];
      sum_out_g += stack_ptr[1];
      sum_out_b += stack_ptr[2];
      sum_out_a += stack_ptr[3];
      sum_in_r -= stack_ptr[0];
      sum_in_g -= stack_ptr[1];
      sum_in_b -= stack_ptr[2];
      sum_in_a -= stack_ptr[3];
    }
  }

  cairo_surface_mark_dirty(rendered);
  delete[] stack;
}

#elif defined(USE_SVGREN)

/**
 * @internal
 * @details
 *
 * "box blur" by Ivan Gagis <igagis@gmail.com>
 * svgren project.

The MIT License (MIT)

Copyright (c) 2015 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
void uxdevice::draw_buffer_t::blur_image(const unsigned int radius) {
  cairo_surface_t *blurred = build_blur_image(radius);
  cairo_surface_destroy(rendered);
  rendered = blurred;
}

/**
 * @internal
 * @fn blur_image
 * @param unsigned int radius
 * @brief
 */
cairo_surface_t *
uxdevice::draw_buffer_t::build_blur_image(unsigned int radius) {
  std::array<double, 2> stdDeviation = {static_cast<double>(radius),
                                        static_cast<double>(radius)};
  cairo_surface_t *ret = cairo_image_surface_blur(rendered, stdDeviation);
  cairo_surface_mark_dirty(ret);
  return ret;
}

/**
 * @internal
 * @fn box_blur_horizontal
 * @param std::uint8_t *dst
 * @param const std::uint8_t *src
 * @param unsigned dstStride
 * @param unsigned srcStride
 * @param unsigned width
 * @param unsigned height
 * @param unsigned boxSize
 * @param unsigned boxOffset
 * @param unsigned channel
 * @brief
 */
void uxdevice::draw_buffer_t::box_blur_horizontal(
  std::uint8_t *dst, const std::uint8_t *src, unsigned dstStride,
  unsigned srcStride, unsigned width, unsigned height, unsigned boxSize,
  unsigned boxOffset, unsigned channel) {
  if (boxSize == 0) {
    return;
  }
  for (unsigned y = 0; y != height; ++y) {
    unsigned sum = 0;
    for (unsigned i = 0; i != boxSize; ++i) {
      int pos = i - boxOffset;
      pos = std::max(pos, 0);
      pos = std::min(pos, int(width - 1));
      sum += src[(srcStride * y) + (pos * sizeof(std::uint32_t)) + channel];
    }
    for (unsigned x = 0; x != width; ++x) {
      int tmp = x - boxOffset;
      int last = std::max(tmp, 0);
      int next = std::min(tmp + boxSize, width - 1);

      dst[(dstStride * y) + (x * sizeof(std::uint32_t)) + channel] =
        sum / boxSize;

      sum += src[(srcStride * y) + (next * sizeof(std::uint32_t)) + channel] -
             src[(srcStride * y) + (last * sizeof(std::uint32_t)) + channel];
    }
  }
}

/**
 * @internal
 * @fn box_blur_vertical
 * @param std::uint8_t *dst
 * @param const std::uint8_t *src
 * @param unsigned dstStride
 * @param unsigned srcStride
 * @param unsigned width
 * @param unsigned height
 * @param unsigned boxSize
 * @param unsigned boxOffset
 * @param unsigned channel
 * @brief
 */
void uxdevice::draw_buffer_t::box_blur_vertical(
  std::uint8_t *dst, const std::uint8_t *src, unsigned dstStride,
  unsigned srcStride, unsigned width, unsigned height, unsigned boxSize,
  unsigned boxOffset, unsigned channel) {
  if (boxSize == 0) {
    return;
  }
  for (unsigned x = 0; x != width; ++x) {
    unsigned sum = 0;
    for (unsigned i = 0; i != boxSize; ++i) {
      int pos = i - boxOffset;
      pos = std::max(pos, 0);
      pos = std::min(pos, int(height - 1));
      sum += src[(srcStride * pos) + (x * sizeof(std::uint32_t)) + channel];
    }
    for (unsigned y = 0; y != height; ++y) {
      int tmp = y - boxOffset;
      int last = std::max(tmp, 0);
      int next = std::min(tmp + boxSize, height - 1);

      dst[(dstStride * y) + (x * sizeof(std::uint32_t)) + channel] =
        sum / boxSize;

      sum += src[(x * sizeof(std::uint32_t)) + (next * srcStride) + channel] -
             src[(x * sizeof(std::uint32_t)) + (last * srcStride) + channel];
    }
  }
}

/**
 * @internal
 * @fn cairo_image_surface_blur
 * @param cairo_surface_t *img
 * @param std::array<double, 2> stdDeviation
 */
cairo_surface_t *uxdevice::draw_buffer_t::cairo_image_surface_blur(
  cairo_surface_t *img, std::array<double, 2> stdDeviation) {

  int w = cairo_image_surface_get_width(img);
  int h = cairo_image_surface_get_height(img);
  int stride = cairo_image_surface_get_stride(img);
  std::uint8_t *src = cairo_image_surface_get_data(img);

  std::array<unsigned, 2> d;
  for (unsigned i = 0; i != 2; ++i) {
    d[i] =
      unsigned(float(stdDeviation[i]) * 3 * std::sqrt(2 * (22 / 7)) / 4 + 0.5f);
  }

  cairo_surface_t *ret = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
  std::uint8_t *retData = cairo_image_surface_get_data(ret);

  std::vector<std::uint8_t> tmp(stride * h);

  std::array<unsigned, 3> hBoxSize;
  std::array<unsigned, 3> hOffset;
  std::array<unsigned, 3> vBoxSize;
  std::array<unsigned, 3> vOffset;
  if (d[0] % 2 == 0) {
    hOffset[0] = d[0] / 2;
    hBoxSize[0] = d[0];
    hOffset[1] = d[0] / 2 - 1; // it is ok if d[0] is 0 and -1 will give a large
    // number because box size is also 0 in that case
    // and blur will have no effect anyway
    hBoxSize[1] = d[0];
    hOffset[2] = d[0] / 2;
    hBoxSize[2] = d[0] + 1;
  } else {
    hOffset[0] = d[0] / 2;
    hBoxSize[0] = d[0];
    hOffset[1] = d[0] / 2;
    hBoxSize[1] = d[0];
    hOffset[2] = d[0] / 2;
    hBoxSize[2] = d[0];
  }

  if (d[1] % 2 == 0) {
    vOffset[0] = d[1] / 2;
    vBoxSize[0] = d[1];
    vOffset[1] = d[1] / 2 - 1; // it is ok if d[0] is 0 and -1 will give a large
    // number because box size is also 0 in that case
    // and blur will have no effect anyway
    vBoxSize[1] = d[1];
    vOffset[2] = d[1] / 2;
    vBoxSize[2] = d[1] + 1;
  } else {
    vOffset[0] = d[1] / 2;
    vBoxSize[0] = d[1];
    vOffset[1] = d[1] / 2;
    vBoxSize[1] = d[1];
    vOffset[2] = d[1] / 2;
    vBoxSize[2] = d[1];
  }

  for (auto channel = 0; channel != 4; ++channel) {
    box_blur_horizontal(tmp.data(), src, stride, stride, w, h, hBoxSize[0],
                        hOffset[0], channel);
  }
  for (auto channel = 0; channel != 4; ++channel) {
    box_blur_horizontal(retData, tmp.data(), stride, stride, w, h, hBoxSize[1],
                        hOffset[1], channel);
  }
  for (auto channel = 0; channel != 4; ++channel) {
    box_blur_horizontal(tmp.data(), retData, stride, stride, w, h, hBoxSize[2],
                        hOffset[2], channel);
  }
  for (auto channel = 0; channel != 4; ++channel) {
    box_blur_vertical(retData, tmp.data(), stride, stride, w, h, vBoxSize[0],
                      vOffset[0], channel);
  }
  for (auto channel = 0; channel != 4; ++channel) {
    box_blur_vertical(tmp.data(), retData, stride, stride, w, h, vBoxSize[1],
                      vOffset[1], channel);
  }
  for (auto channel = 0; channel != 4; ++channel) {
    box_blur_vertical(retData, tmp.data(), stride, stride, w, h, vBoxSize[2],
                      vOffset[2], channel);
  }

  return ret;
}

#endif
