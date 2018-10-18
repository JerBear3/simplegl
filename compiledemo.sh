#!/bin/bash
g++ -s -Os -Wall -Wl,-rpath . -ffast-math -no-pie -o demo demo.cpp -L. -lglfw -ldl -lsimplegl
