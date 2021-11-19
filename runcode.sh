#!bin/bash/


g++ -Isrc/Include -Lsrc/lib -o main main.cpp Complex.cpp qtree.cpp -lmingw32 -lSDL2main -lSDL2;
./main.exe
