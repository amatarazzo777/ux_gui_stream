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
 * @file ux_pipeline_order.hpp
 * @date 9/7/20
 * @version 1.0
 * @brief names the stages in the pipeline. Each of the abstract class template
 * parameters when they are mentioned on the unit definition should name one of
 * these within its parameter.
 */
namespace uxdevice {
const int order_init = 1;
const int order_layout_option = 2;
const int order_render_option = 3;

const int order_before_render = 4;
const int order_render = 5;
const int order_after_render = 6;
const int order_terminate = 7;

} // namespace uxdevice
