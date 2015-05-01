CPP =g++
CPPFLAGS =-Wall -O2 -std=gnu++0x
LDLIBS =  	-L/usr/lib 		 \
			-L/usr/X11R6/lib \
			-lX11			 \
			-lGL			 \
			-lGLU			 \
			-lglut			 \
			-ljpeg

SOURCE =main.cpp utils.cpp noise.cpp
BIN =main

all:
	$(CPP) $(CPPFLAGS) -o $(BIN) $(SOURCE) $(LDLIBS)

clean:
	rm -rf $(BIN)