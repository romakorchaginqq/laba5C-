CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: program

program: main.o Product.o
 $(CXX) $(CXXFLAGS) -o program main.o Product.o

main.o: main.cpp Product.h
 $(CXX) $(CXXFLAGS) -c main.cpp

Product.o: Product.cpp Product.h
 $(CXX) $(CXXFLAGS) -c Product.cpp

clean:
 rm -f *.o program

run: program
 ./program

# Запуск с указанием файла
run_test: program
 ./program test.txt