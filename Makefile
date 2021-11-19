
output: main.o
	g++ main.o -o output

main.o: main.cpp
	g++ -Isrc/Include -Lsrc/lib -o main main.cpp Complex.cpp qtree.cpp -lmingw32 -lSDL2main -lSDL2;

clean:
	rm *.o