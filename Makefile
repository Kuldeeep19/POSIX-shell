.PHONY: all clean

all: main

main: *.cpp
	g++ -std=c++17 -Wall *.cpp -o main

clean:
	rm -f main