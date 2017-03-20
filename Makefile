CC=g++
FLAGS=-std=c++11 -pthread

MAIN=main

all:main

main: main.cpp
	    $(CC) $(FLAGS) main.cpp -o main

clean:
	    rm main


