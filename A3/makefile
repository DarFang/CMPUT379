CC	= g++
CFLAGS	= -Wall


all: CtoS
CtoS: client server
	

client: client.o 
	$(CC) $(CFLAGS) -o client client.o 
client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp -o client.o 
	
server: server.o
	$(CC) $(CFLAGS) -o server server.o 
server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp -o server.o 

man: server.man client.man
	groff -Tpdf -man server.man >server.pdf
	groff -Tpdf -man client.man >client.pdf

clean:
	rm *.o

cleanCompile:
	rm client server

compress:
	zip dfang1_CMPUT379_A3.zip *.cpp makefile README inputtest1 *.h *.man

