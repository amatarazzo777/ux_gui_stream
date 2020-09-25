CC=clang
#CC=g++
CFLAGS=-std=c++17 -Os 
INCLUDES=-Iheaders/ `pkg-config --cflags cairo pango pangocairo  librsvg-2.0` -fexceptions

LFLAGS=`pkg-config --libs cairo pango pangocairo  librsvg-2.0` 

debug: CFLAGS += -g
debug: vis.out

release: LFLAGS += -s
release: vis.out


all: vis.out


vis.out: main.o ux_device.o ux_display_context.o ux_display_unit_base.o \
	ux_display_units.o ux_draw_buffer.o ux_painter_brush.o ux_unit_memory.o \
	ux_unit_memory_visitors.o
	$(CC) -o vis.out main.o ux_device.o ux_display_context.o ux_display_unit_base.o \
	ux_display_units.o ux_draw_buffer.o ux_painter_brush.o ux_unit_memory.o \
	ux_unit_memory_visitors.o

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c main.cpp -o main.o

ux_device.o: ux_device.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_device.cpp -o ux_device.o
	
ux_display_context.o: ux_display_context.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_display_context.cpp -o ux_display_context.o

ux_display_unit_base.o: ux_display_unit_base.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_display_unit_base.cpp -o ux_display_unit_base.o

ux_display_units.o: ux_display_units.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_display_units.cpp -o ux_display_units.o

ux_draw_buffer.o: ux_draw_buffer.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_draw_buffer.cpp -o ux_draw_buffer.o

ux_painter_brush.o: ux_painter_brush.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_painter_brush.cpp -o ux_painter_brush.o

ux_unit_memory.o: ux_unit_memory.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_unit_memory.cpp -o ux_unit_memory.o

ux_unit_memory_visitors.o: ux_unit_memory_visitors.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c ux_unit_memory_visitors.cpp -o ux_unit_memory_visitors.o

clean:
	rm *.o *.out

