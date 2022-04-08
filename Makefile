
CC=g++ -std=c++11
CFLAGS=-Wall -g -Wno-unused-variable -pthread
LDFLAGS=-lglfw -lglew -framework OpenGL

all: app

app: src/main.cpp src/app/*.cpp src/shaders/*.cpp src/libs/*.cpp src/utilities/*.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


clean:
	$(RM) app
	$(RM) *.o