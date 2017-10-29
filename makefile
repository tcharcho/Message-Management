OPTIONS = -ansi -g -Wall

all: run post.o addauthor.o
	gcc post.o -o post -L. -lstream -lmysqlclient -L/usr/lib/x86_64-linux-gnu/;\
	gcc addauthor.o -o addauthor -L. -lstream -lmysqlclient -L/usr/lib/x86_64-linux-gnu/;\
	gcc $(OPTIONS) config.c -o config;\
	gcc $(OPTIONS) db.c -o db -lmysqlclient -L/usr/lib/x86_64-linux-gnu/;\
	mkdir -p messages;\
	chmod 770 messages/;\
	chmod +x getStreams.py;\
	chmod +x getLast.py;\
	chmod +x getTotal.py;\
	chmod +x markRead.py;\
	chmod +x getPost.py;\
	chmod +x allRead.py

run: a1
	./a1 post.cc

a1: a1.o helper.o
	gcc $(OPTIONS) a1.o helper.o -o a1

a1.o: a1.c a1.h
	gcc $(OPTIONS) -c a1.c -o a1.o

helper.o: helper.c a1.h
	gcc $(OPTIONS) -c helper.c -o helper.o


post.o: post.c libstream.a
	gcc $(OPTIONS) -c post.c -o post.o
	
addauthor.o: addauthor.c libstream.a
	gcc $(OPTIONS) -c addauthor.c -o addauthor.o

libstream.a: stream.c stream.h
	gcc $(OPTIONS) -c stream.c -o stream.o;\
	ar cr libstream.a stream.o

clean: 
	rm -f -r *.o *.dSYM post addauthor a1 libstream.a post.c messages config db

