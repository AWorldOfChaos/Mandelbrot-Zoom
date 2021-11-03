# Mandelbrot Zoom

This project was done as a part of the CS293 course : Data Structures and Algorithms Lab at IIT Bombay in the year 2021-22.


# What is the Mandelbrot Zoom

The Mandelbrot set is an infinititely complex, self similar shape on a two dimentional plane. It is impossible to precisely render such a shape with discrete pixels. Thus, in order to see the self similarity of finer and finer detail, we create animations where each frame shows a smaller and smaller portion of the plane. This changing of scale is called a zoom. Mandelbrot zooms are beautiful and intricate

## Formal Definition

The Mandelbrot set is the set of complex numbers *c* for which the function 
> f<sub>c</sub>(z)=z<sup>2</sup>+c

does not diverge to infinity when iterated from *z=0*, i.e., for which the sequence 
> f<sub>c</sub>(z), f<sub>c</sub>(f<sub>c</sub>(z)), ..

remains bounded in absolute value.

# Some Implementation Details

I have used the [SDL library](https://wiki.libsdl.org/), which is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL for rendering the Mandelbrot set. There is an underlying mapping which connects location on the screen to the location which I have modelled on the [Arduino map function](https://www.arduino.cc/reference/en/language/functions/math/map/) with modifications to allow a more continuous output.

# Running the Code on your Machine
To run the code, simply run the following:  ```bash runcode.sh```.
If due to some reason that does not work, run the following commands.
```
g++ -Isrc/Include -Lsrc/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2;
./main.exe
```