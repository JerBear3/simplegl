#!/bin/bash
g++ -s -Os -Wall -ffast-math -fPIC -shared -o libsimplegl.so lib/*.c* math/*.c* gl/*.c* -lglfw -ldl
