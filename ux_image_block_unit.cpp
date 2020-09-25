

/**
\internal
\fn pipeline

\brief create the image rendering pipeline

 */
void uxdevice::image_block_storage_t::pipeline(cairo_t *cr, coordinate_t *a) {

  /**
    \details The cached rendering function has not been established,
    create the image rendering pipeline consisting of individualized lambda
    functions. This functionality can provide image processing capabilities
    to produce visual effects. Using the emplace back to invoke operations
    on the image->block_ptr;
  */

  /// @brief these steps are common in the first part of the image rendering
  pipeline_push<order_render_init>([&]() { matrix.emit(cr); });

  // compute pipeline that includes rendering commands. The rendering commands
  // are sequenced and appropriate fill, preserve order is maintained.
  pipeline_push_visit(cairo_coordinate_visitor_t{cr, a});

  /// @brief additional processing can also be inserted here. or
  /// may be inserted as a new unit_memory emitter accepting a visitor type.

  // add result to buffer
  pipeline_push<order_render>([&]() { image_block.emit(cr, a); });

  return;
}

void uxdevice::image_block_t::is_valid() {
  // validate required parameters
  if (!access_pipeline_memory<coordinate_t>() || description.size() == 0) {
    const char *s = "An image_block_t object must include the following "
                    "attributes. coordinate_t and an idescription name.";
    error_report(s);
    auto fn = [=](display_context_t *context) {};

    fn_base_surface = std::bind(fn, _1);
    fn_cache_surface = std::bind(fn, _1);
    fn_draw = std::bind(fn, _1);
    fn_draw_clipped = std::bind(fn, _1);
    return;
  }
}

/**
\internal
\brief
*/
void uxdevice::image_block_t::emit(display_context_t *context) {
  using namespace std::placeholders;

  if (is_processed)
    return;

  if (!is_valid())
    return;

  // set the ink area.
  coordinate_t &a = *access_pipeline_memory<coordinate_t>();

  // this function is geared for a thread of an image loading.
  // however, it is not implemented as a thread currently, just named for
  // future.
  auto fnthread = [&]() {
    image_block = draw_buffer_t(description, coordinate->w, coordinate->h);

    if (image_block.format == draw_buffer_format_t::none) {
      const char *s = "The image_block_t could not be processed or loaded. ";
      error_report(s);
      error_report(description);

    } else {
      set_ink(ink_rectangle);
    }
  };

  fnthread();

  is_processed = true;
  state_hash_code();
}
