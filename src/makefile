# A makefile.

#SRCS=main.cpp demo.cpp demo_part.cpp sync_track.cpp sync.cpp parts/part_logo.cpp parts/part_vapor1.cpp parts/part_trap1.cpp music.cpp demo_config.cpp
SRCS=$(wildcard *.cpp) $(wildcard */*.cpp)
TARGET?=demo
OBJS=$(SRCS:.cpp=.o)
CFLAGS+=-O3 -MMD -MP -Wall
debug:CFLAGS=-g -pg -MMD -MP -DDEBUG_BUILD
INCLUDES+=-I./ -Ilib3dapp/include/ -Iparts/
LFLAGS+=-Llib3dapp/
LIBS?=-lm -lGL -lSDL2 -lGLEW -l3dapp
LIBS_CUSTOM=
CC=g++

raspi:CFLAGS=-O3 -MMD -MP -Wall -DRASPI_BUILD
raspi:INCLUDES=-I/opt/vc/include/ -I./ -Ilib3dapp/include -Iparts/
raspi:LFLAGS=-L/opt/vc/lib -Llib3dapp/
raspi:LIBS=-lm -lGLESv2 -lEGL -lbcm_host -lSDL -l3dapp

all: $(TARGET)
	@echo "Finished."
	
debug: $(TARGET)
	@echo "Finished."

raspi: $(TARGET)
	@echo "Finished."

$(TARGET): $(OBJS)
	cd lib3dapp; $(MAKE) $(MAKECMDGOALS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS) $(LIBS_CUSTOM)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
clean:
	cd lib3dapp; $(MAKE) clean
	find ./ -type f -name "*.o" -delete
	find ./ -type f -name "*.d" -delete
	rm -f $(TARGET)
	rm -f $(TARGET).exe

-include *.d
