#CC=clang
CC=g++
CFLAGS=-std=c++17 -Os -Weffc++ -Wall

INCLUDES=-Iheaders/ `pkg-config --cflags cairo pango pangocairo  librsvg-2.0` -fexceptions

LFLAGS=-lpthread -lpthread -lm -lX11-xcb -lX11 -lxcb -lxcb-image -lxcb-keysyms -lstdc++ \
	`pkg-config --libs cairo pango pangocairo librsvg-2.0` 

debug: CFLAGS += -g
debug: vis.out

release: LFLAGS += -s
release: vis.out

all: vis.out

vis.out: main.o ux_device.o ux_coordinate.o ux_display_context.o \
	ux_display_visual.o ux_draw_buffer.o ux_drawing_unit_primitives.o \
	ux_image_block_unit.o ux_painter_brush.o ux_pipeline_memory.o \
	ux_surface_area_units.o ux_textual_render.o ux_text_units.o
	$(CC) $(LFLAGS) -o vis.out main.o ux_device.o ux_coordinate.o ux_display_context.o \
	ux_display_visual.o ux_draw_buffer.o ux_drawing_unit_primitives.o \
	ux_image_block_unit.o ux_painter_brush.o ux_pipeline_memory.o \
	ux_surface_area_units.o ux_textual_render.o ux_text_units.o


main.o: main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c main.cpp -o main.o

ux_device.o: ux_device.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_device.cpp -o ux_device.o
	
ux_display_context.o: ux_display_context.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_display_context.cpp -o ux_display_context.o

ux_display_visual.o : ux_display_visual.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_display_visual.cpp -o ux_display_visual.o

ux_draw_buffer.o: ux_draw_buffer.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_draw_buffer.cpp -o ux_draw_buffer.o

ux_drawing_unit_primitives.o: ux_drawing_unit_primitives.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_drawing_unit_primitives.cpp -o ux_drawing_unit_primitives.o

ux_coordinate.o: ux_coordinate.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_coordinate.cpp -o ux_coordinate.o

ux_image_block_unit.o: ux_image_block_unit.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_image_block_unit.cpp -o ux_image_block_unit.o

ux_painter_brush.o: ux_painter_brush.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_painter_brush.cpp -o ux_painter_brush.o

ux_pipeline_memory.o: ux_pipeline_memory.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_pipeline_memory.cpp -o ux_pipeline_memory.o

ux_surface_area_units.o: ux_surface_area_units.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_surface_area_units.cpp -o ux_surface_area_units.o

ux_textual_render.o: ux_textual_render.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_textual_render.cpp -o ux_textual_render.o

ux_text_units.o: ux_text_units.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_text_units.cpp -o ux_text_units.o


clean:
	rm *.o *.out

