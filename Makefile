COMPILER = clang++
STD = -std=c++20
OUT = main

all: build

build: clear
	$(COMPILER) $(STD) src/main.cpp src/bmp.cpp -o bin/$(OUT)

clear:
	rm -f bin/*