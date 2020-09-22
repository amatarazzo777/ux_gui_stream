/*
 * ux_unit_memory_visitors.cpp
 *
 *  Created on: Sep 21, 2020
 *      Author: anthony
 */


#include <ux_device.hpp>

void uxdevice::cairo_coordinate_visitor_t::accept(void) {
if (fn_v2)
fn_v2(cr, coord);
}

void uxdevice::cairo_coordinate_visitor_t::emit(cairo_t *_cr) {
if (fn_v1)
fn_v1(_cr);
}

void uxdevice::cairo_coordinate_visitor_t::emit(cairo_t *_cr, coordinate_t &a) {
if (fn_v2)
fn_v2(_cr, a);
}
