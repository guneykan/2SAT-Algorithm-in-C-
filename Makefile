all: main.o

main.o: main.cc
	g++ --std=c++11 main.cc -o project3
clean:
	rm -rf *o  project3
