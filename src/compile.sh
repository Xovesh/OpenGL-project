gcc *.c -lGL -lGLU -lglut -lm `pkg-config --libs --cflags gtk+-3.0` -lpthread -rdynamic -o output
