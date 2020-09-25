/**

\class antialias_t
\brief

\details


*/
void uxdevice::antialias_t::emit(cairo_t *cr) {
  cairo_set_antialias(cr, static_cast<cairo_antialias_t>(value));
}

/**

\class line_width_t
\brief sets the line width when used during a stroke path operation.
This includes text and line drawing.

\details


 */
void uxdevice::line_width_t::emit(cairo_t *cr) {
  cairo_set_line_width(cr, value);
}

/**

\class line_cap_t
\brief

\details


*/
void uxdevice::line_cap_t::emit(cairo_t *cr) {
  cairo_set_line_cap(cr, static_cast<cairo_line_cap_t>(value));
}

/**

\class line_join_t
\brief

\details


*/
void uxdevice::line_join_t::emit(cairo_t *cr) {
  cairo_set_line_join(cr, static_cast<cairo_line_join_t>(value));
}
/**

\class miter_limit_t
\brief

\details


 */
void uxdevice::miter_limit_t::emit(cairo_t *cr) {
  cairo_set_miter_limit(cr, value);
}
/**

\class line_dash_storage_t - friend of line_deshes_t
\brief

\details


 */
void uxdevice::line_dashes_t::emit(cairo_t *cr) {
  cairo_set_dash(cr, value.data(), value.size(), offset);
}

/**

\class tollerance_t
\brief

\details


 */
void uxdevice::tollerance_t::emit(cairo_t *cr) {
  cairo_set_tolerance(cr, value);
}

/**

\class graphic_operator
\brief

\details


*/
void uxdevice::graphic_operator_t::emit(cairo_t *cr) {
  cairo_set_operator(cr, static_cast<cairo_operator_t>(value));
}

/**

\class arc_storage_t
\brief

\details


*/
void uxdevice::arc_t::emit(cairo_t *cr) {
  cairo_arc(cr, xc, yc, radius, angle1, angle2);
}

/**

\class negative_arc_t
\brief

\details


 */
void uxdevice::negative_arc_t::emit(cairo_t *cr) {
  cairo_arc_negative(cr, xc, yc, radius, angle1, angle2);
}

/**

\class curve_storage_t
\brief

\details


 */
void uxdevice::curve_t::emit_relative(cairo_t *cr) {
  cairo_rel_curve_to(cr, x1, y1, x2, y3, x3, y3);
}

void uxdevice::curve_t::emit_absolute(cairo_t *cr) {
  cairo_curve_to(cr, x1, y1, x2, y2, x3, y3);
}

/**

\class line_t
\brief

\details


 */
void uxdevice::line_t::emit_relative(cairo_t *cr) {
  cairo_rel_line_to(cr, x, y);
}

void uxdevice::line_t::emit_absolute(cairo_t *cr) { cairo_line_to(cr, x, y); }

/**

\class hline_t
\brief

\details


 */

void uxdevice::hline_t::emit_relative(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_rel_line_to(cr, value, 0);
  }
}

void uxdevice::hline_t::emit_absolute(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_line_to(cr, value, cury);
  }
}
/**

\class vline_t
\brief

\details


 */

void uxdevice::vline_t::emit_relative(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_rel_line_to(cr, 0, value);
  }
}

void uxdevice::vline_t::emit_absolute(cairo_t *cr) {
  if (cairo_has_current_point(cr)) {
    double curx = 0.0, cury = 0.0;
    cairo_get_current_point(cr, &curx, &cury);
    cairo_line_to(cr, curx, value);
  }
}
/**

\class rectangle_t
\brief

\details


 */
void uxdevice::rectangle_t::emit(cairo_t *cr) {
  cairo_rectangle(cr, x, y, width, height);
}
/**

\class close_path_t
\brief

\details


 */
void uxdevice::close_path_t::emit(cairo_t *cr) { cairo_close_path(cr); }

/**

\class stroke_path_t
\brief

\details


*/
void uxdevice::stroke_path_t::emit(cairo_t *cr) {
  painter_brush_t::emit(cr);
  cairo_stroke(cr);
}

/**

\class fill_path_t
\brief

\details


 */
void uxdevice::fill_path_t::emit(cairo_t *cr) {
  painter_brush_t::emit(cr);
  cairo_fill(cr);
}

/**

\class stroke_fill_path_t
\brief

\details


 */
void uxdevice::stroke_fill_path_t::emit(cairo_t *cr) {
  stroke_brush.emit(cr);
  cairo_stroke_preserve(cr);
  fill_brush.emit(cr);
  cairo_fill(cr);
}

/**

\class mask_t
\brief

\details


 */
void uxdevice::mask_t::emit(cairo_t *cr) {}

/**

\class paint_t
\brief

\details


 */
void uxdevice::paint_t::emit(cairo_t *cr) {
  if (value == 1.0) {
    cairo_paint(cr);
  } else {
    cairo_paint_with_alpha(cr, value);
  }
}
/**
