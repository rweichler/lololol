CC=g++ -Wall

LongInt.out : LongIntMain.o LongInt.o
	$(CC) -o LongInt.out LongIntMain.o LongInt.o

LongIntMain.o : LongIntMain.cpp LongInt.h
	$(CC) -c LongIntMain.cpp

LongInt.o : LongInt.cpp LongInt.h
	$(CC) -c LongInt.cpp
