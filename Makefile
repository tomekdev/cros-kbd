OBJS=cros-kbd.o \
     backlight.o \
     volume.o

CPPFLAGS=-g
LDFLAGS=-g

all: $(OBJS)
	g++ -o cros-kbd $(OBJS) $(CPPFLAGS) $(LDFLAGS)

clean:
	rm -rf $(OBJS) cros-kbd
