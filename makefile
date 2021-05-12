bin/mps		: mpsc.o
			g++ -O2 mpsc.o -o bin/mps

mpsc.o		: src/mpsc.cpp
			g++ -c -O2 src/mpsc.cpp

clean		: 
			rm -rf *.o bin/*