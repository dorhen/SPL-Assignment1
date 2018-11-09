CC = g++
CFLAGS = -g -Wall -std=c++11
LFLAGS = -L/usr/lib

all: rest

rest: bin/main.o bin/Restaurant.o bin/Action.o bin/Customer.o bin/Table.o bin/Dish.o
	@echo 'Building: rest'
	$(CC) -o bin/rest bin/main.o bin/Restaurant.o bin/Action.o bin/Customer.o bin/Table.o bin/Dish.o $(LFLAGS)
	@echo 'Building rest finished successfuly.'
	@echo ' '

bin/main.o: src/main.cpp src/Restaurant.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

bin/Restaurant.o: src/Restaurant.cpp  include/Restaurant.h src/Table.cpp src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Table.o src/Table.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Action.o: src/Action.cpp include/Action.h src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/Customer.o: src/Customer.cpp include/Customer.h src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp

bin/Dish.o: src/Dish.cpp include/Dish.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp

bin/Table.o: src/Table.cpp include/Table.h src/Dish.cpp src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Table.o src/Table.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp
clean:
	rm -f bin/*
