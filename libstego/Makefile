 #Makefile for Hello World project
# archive -- static
CC=gcc
CXX=g++
CFLAGS=-Whello -L. -lworld

#static
bitarray_st: libbitarray.a
#	$(CC) -o hello main.o -L. -lhello
#dinamic
#bitarray_dn: main.o libhello.so
#	$(CC) -o hello main.o -L. -lhello -Wl,-rpath,.
#main.o: main.c
#	$(CC) -c main.c

libstego.a: Array.o BitArray.o Exception.o StegoDecoder.o OggStegoDecoder.o StegoEncoder.o OggStegoEncoder.o
	ar cr libstego.a Array.o BitArray.o Exception.o StegoDecoder.o OggStegoDecoder.o StegoEncoder.o OggStegoEncoder.o
libbitarray.a: Array.o BitArray.o Exception.o
	ar cr libbitarray.a Array.o BitArray.o Exception.o
libbitarray.so: Array.o BitArray.o Exception.o
	$(CXX) -shared -o libbitarray.so Array.o BitArray.o Exception.o
OggStegoEncoder.o: OggStegoEncoder.cpp
	$(CXX) -c -fPIC OggStegoEncoder.cpp
StegoEncoder.o: StegoEncoder.cpp
	$(CXX) -c -fPIC StegoEncoder.cpp
OggStegoDecoder.o: OggStegoDecoder.cpp
	$(CXX) -c -fPIC OggStegoDecoder.cpp
StegoDecoder.o: StegoDecoder.cpp
	$(CXX) -c -fPIC StegoDecoder.cpp
BitArray.o: BitArray.cpp
	$(CXX) -c -fPIC BitArray.cpp
Array.o: Array.cpp
	$(CXX) -c -fPIC Array.cpp
Exception.o: Exception.cpp
	$(CXX) -c -fPIC Exception.cpp

clean:
	rm -f *.o *.a *.so *~ hello
