#!/bin/sh

gcc -g playerr.c -o playerr
gcc -g leader.c -o leader
gcc -o opengl opengl.c -lGL -lGLU -lglut -lm
gcc -g parent.c -o parent



./parent
