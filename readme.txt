



The cairo, librsvg-2.0, xcb, pango rendering chain. Currently within
 the implementation, X11, X11-xcb in combination for the keyboard processing.
 That is their are a massive amount of keyboards to account for. There are a
 few good source code bases such as the "google chrome" which have well tested
 keyboard processing, firefox. SDL is basic for input and requires touchup
 processing to provide gui style user input mechanism. That also seems to be
 the issue with the XCB. It was difficult to detect upper and lower case as
 well as matching numbers, along with the shift key. A basic keyboard. This
 area of the system will need a better method. The chrome project uses the X11
 keyboard routine as well.
 
 @note: on linux, xcb the following dependencies are needed. These require
 administrator privilege and use the 'sudo' on ubuntu style.
 
  # apt-get install libgtk-3-dev libglib2.0-dev libpixman-1-dev \
  		libcairo2-dev libglib2.0-dev  libpixman-1-dev libpango1.0-dev \
  		libpangocairo-1.0-0 libpangoft2-1.0-0 libfreetype-dev \
  		libx11-dev libxcb-keysyms1-dev \
  		libx11-dev libx11-xcb-dev
 
  documentation:
 
  #  apt-get install  libcairo2-doc libpango1.0-doc
 
 currently this is the only one implemented.

pkg-config --cflags cairo pangocairo pango pixman-1 X11 X11-xcb xcb \
	xcb-image xcb-keysyms xcb-shm librsvg-2.0 gdk-pixbug-2.0 gio-2.0 \
	glib-2.0 gobject-2.0
	
pkg-config --libs cairo pangocairo pango pixman-1 X11 X11-xcb xcb \
	xcb-image xcb-keysyms xcb-shm librsvg-2.0 gdk-pixbug-2.0 gio-2.0 \
	glib-2.0 gobject-2.0
		

cairo

pango

pangocairo

pixman-1

X11
X11-xcb
xcb
xcb-image
xcb-keysyms
xcb-shm


librsvg-2.0
gdk-pixbug-2.0
gio-2.0
glib-2.0
gobject-2.0



