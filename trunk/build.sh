#!/bin/bash
gcc -L/usr/X11R6/lib -L/usr/local/lib -I/usr/X11R6/include -I/usr/local/include -lX11 -fPIC -c *.c
gcc -shared -Wl,-soname,ude-lib-xprop.so -o ude-lib-xprop.so *.o
