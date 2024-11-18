Headers := include/akinator.h
Objects := obj/main.o obj/akinator.o

akinator.out: ${Objects} ${Headers}
	g++ ${Objects} -o akinator.out

obj/main.o: src/main.cpp ${Headers}
	g++ -c src/main.cpp -o obj/main.o

obj/akinator.o: src/akinator.cpp ${Headers}
	g++ -c src/akinator.cpp -o obj/akinator.o

run:
	./akinator.out
