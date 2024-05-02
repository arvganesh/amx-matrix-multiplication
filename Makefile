CFLAGS = -std=c++20 -O3 -Rpass=vector -fopenmp-simd -ffast-math -march=armv8-a+fp+simd

run: MMBenchmark.o Kernels.o
	g++ -o run MMBenchmark.o Kernels.o $(CFLAGS)

MMBenchmark.o: MMBenchmark.cpp
	g++ -c MMBenchmark.cpp $(CFLAGS)

test: MMTest.o Kernels.o
	g++ -o test MMTest.o Kernels.o $(CFLAGS)

MMTest.o: MMTest.cpp
	g++ -c MMTest.cpp $(CFLAGS)

Kernels.o: Kernels.cpp Kernels.h MMKernel.h Matrix.h
	g++ -c Kernels.cpp $(CFLAGS)

cache:
	valgrind --tools=cachegrind ./run tiled seed random


clean:
	rm *.o *.h.gch run
