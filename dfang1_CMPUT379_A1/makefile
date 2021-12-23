CC	= g++
CFLAGS	= -Wall
OBJECTS = shell379.o shellJobs.o shellparseString.o

all: shell379.o shellJobs.o shellparseString.o link cleanObj

shell379.o: shell379.cpp
	$(CC) $(CFLAGS) -c shell379.cpp -o shell379.o

shellJobs.o: shellJobs.cpp
	$(CC) $(CFLAGS) -c shellJobs.cpp -o shellJobs.o

shellparseString.o: shellparseString.cpp
	$(CC) $(CFLAGS) -c shellparseString.cpp -o shellparseString.o

link: $(OBJECTS)
	$(CC) -o shell $(OBJECTS)

cleanObj:
	rm *.o

clean:
	rm shell

compress:
	zip dfang1_CMPUT379_A1.zip *.cpp *makefile *.h

