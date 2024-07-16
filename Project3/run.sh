#!/bin/sh

gcc -g main.c -o main -lpthread -lrt
gcc -g liquidline.c -o liquidline -lpthread -lrt
gcc -g pillline.c -o pillline -lpthread -lrt
gcc -g manager.c -o manager -lpthread -lrt
gcc -g timer.c -o timer -lpthread -lrt
gcc -o opengl opengl.c -lGL -lGLU -lglut -lm

./main