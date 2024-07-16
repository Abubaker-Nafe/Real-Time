#!/bin/sh

gcc -g main.c -o main
gcc -g plane.c -o plane
gcc -g collectingworker.c -o collectingworker
gcc -g splittingworker.c -o splittingworker
gcc -g distributionworker.c -o distributionworker
gcc -g families.c -o families
gcc -g soldier.c -o soldier
gcc -g containers.c -o containers
gcc -o opengl opengl.c -lGL -lGLU -lglut -lm
gcc -g timer.c -o timer

./main