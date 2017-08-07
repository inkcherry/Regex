lregex:main.o	adj.o	dfa.o	edge.o	vertex.o
	g++ -g main.o	adj.o	dfa.o	edge.o	vertex.o -o lregex
main.o:src/main.cpp	src/dfa.hpp src/adj.hpp
	g++ -std=c++11 -g -c src/main.cpp
adj.o:src/adj.cpp src/adj.hpp src/vertex.hpp
	g++ -std=c++11 -g -c src/adj.cpp
dfa.o:src/dfa.cpp src/dfa.hpp src/edge.hpp src/vertex.hpp src/adj.hpp
	g++ -std=c++11 -g -c src/dfa.cpp
edge.o:src/edge.cpp src/edge.hpp
	g++ -std=c++11 -g -c src/edge.cpp
vertex.o:src/vertex.cpp src/vertex.hpp src/edge.hpp
	g++ -std=c++11 -g -c src/vertex.cpp
clean:
	rm -f *.o lregex


