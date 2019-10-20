all: src/main.cpp src/heuristic.cpp
	g++ src/main.cpp src/heuristic.cpp -o relaxlag -Iinc/