

/**

\class coordinate_t
\brief The class is used to hold a location and clipping width, height.

\details

 */

void uxdevice::coordinate_t::emit_relative(cairo_t *cr) {
  cairo_rel_move_to(cr, x, y);
}
void uxdevice::coordinate_t::emit_absolute(cairo_t *cr) {
  cairo_move_to(cr, x, y);
}
void uxdevice::coordinate_t::emit(PangoLayout *layout) {
  if (pango_layout_get_width(layout) != w * PANGO_SCALE)
    pango_layout_set_width(layout, w * PANGO_SCALE);

  if (pango_layout_get_height(layout) != h * PANGO_SCALE)
    pango_layout_set_height(layout, h * PANGO_SCALE);
}

/**
\class relative_coordinate_t
\brief

\details

        */
void uxdevice::relative_coordinate_t::emit(display_context_t &context) {
  context.unit_memory_erase<absolute_coordinate_t>();
}

/**

\class absolute_coordinate_t
\brief

\details


 */
void uxdevice::absolute_coordinate_t::emit(display_context_t &context) {
  context.unit_memory_erase<relative_coordinate_t>();
}
