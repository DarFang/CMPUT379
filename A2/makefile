CC	= g++
CFLAGS	= -Wall
OBJECTS = prodcon.o outputfile.o

all: prodcon.o  outputfile.o link cleanObj

prodcon.o: prodcon.cpp
	$(CC) $(CFLAGS) -c prodcon.cpp -o prodcon.o -lpthread
outputfile.o: outputfile.cpp
	$(CC) $(CFLAGS) -c outputfile.cpp -o outputfile.o 

link: $(OBJECTS)
	$(CC) -o prodcon $(OBJECTS) -lpthread

cleanObj:
	rm *.o

clean:
	rm prodcon

compress:
	zip dfang1_CMPUT379_A2.zip *.cpp makefile README inputexample *.h

