CXXFLAGS=`pkg-config cairo gtk+-2.0 --cflags` -g
LIB=`pkg-config cairo gtk+-2.0 --libs` -pthread

SRCS=main.cc

OBJS=$(SRCS:.cc=.o)

build: prog

prog: clean $(OBJS) dimensional-gtk.xml
	g++ -g $(LIB) $(OBJS) -o prog

dimensional-gtk.xml: dimensional.glade
	gtk-builder-convert dimensional.glade dimensional-gtk.xml

clean:
	rm -f $(OBJS) prog
