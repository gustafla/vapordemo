# A makefile.

SRCS=main.cpp demo.cpp demo_drawable.cpp parts/part_pascal_triangles.cpp
TARGET=demo
OBJS=$(SRCS:.cpp=.o)
CFLAGS=-O2 -MMD -MP
debug:CFLAGS=-g -MMD -MP -DDEBUG_BUILD
INCLUDES=-I./ -I./lib3dapp/ -I./parts/
LFLAGS=-L./lib3dapp/
LIBS=-lGL -lSDL2 -lGLEW -l3dapp
LIBS_CUSTOM=
CC=g++

raspi:CFLAGS=-O2 -MMD -MP -fPIC -DRASPI_BUILD
raspi:INCLUDES=-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/src/hello_pi/libs/ilclient -I/opt/vc/src/hello_pi/libs/vgfont -I./ -I./lib3dapp/ -I./parts/
raspi:LFLAGS=-L/opt/vc/lib -L./lib3dapp/
raspi:LIBS=-lGLESv2 -lEGL -lbcm_host -l3dapp

all: $(TARGET)
	@echo "Finished."
	
debug: $(TARGET)
	@echo "Finished."

raspi: $(TARGET)
	@echo "Finished."

$(TARGET): $(OBJS)
	cd lib3dapp; make $(MAKECMDGOALS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS) $(LIBS_CUSTOM)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
clean:
	cd lib3dapp; make clean
	find ./ -type f -name "*.o" -delete
	find ./ -type f -name "*.d" -delete
	rm -f $(TARGET)

-include *.d
