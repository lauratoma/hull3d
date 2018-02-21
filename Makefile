PLATFORM = $(shell uname)


## Compilation flags
##comment out one or the other 
##debugging 
CFLAGS = -g 
##release
#CFLAGS = -O3 -DNDEBUG
LDFLAGS=

CFLAGS+= -Wall

ifeq ($(PLATFORM),Darwin)
## Mac OS X
CFLAGS += -m64 -isystem/usr/local/include  -Wno-deprecated 
LDFLAGS+= -m64 -lc -framework AGL -framework OpenGL -framework GLUT -framework Foundation
else
## Linux
CFLAGS += -m64
INCLUDEPATH  = -I/usr/include/GL/ 
LIBPATH = -L/usr/lib64 -L/usr/X11R6/lib
LDFLAGS+=  -lGL -lglut -lrt -lGLU -lX11 -lm  -lXmu -lXext -lXi
endif


CC = g++ -O3 -Wall $(INCLUDEPATH)


PROGS = hull3d

default: $(PROGS)

hull3d: hull3d.o geom.o 
	$(CC) -o $@ hull3d.o geom.o $(LDFLAGS)

hull3d.o: hull3d.cpp  geom.h 
	$(CC) -c $(INCLUDEPATH) $(CFLAGS)   hull3d.cpp  -o $@

geom.o: geom.cpp geom.h 
	$(CC) -c $(INCLUDEPATH)  $(CFLAGS)  geom.cpp -o $@

clean::	
	rm *.o
	rm hull3d


