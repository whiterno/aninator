Headers := include/akinator.h include/akinator_dump.h
Objects := obj/main.o obj/akinator.o obj/akinator_dump.o

akinator.out: ${Objects} ${Headers}
	g++ ${Objects} -o akinator.out

obj/main.o: src/main.cpp ${Headers}
	g++ -c src/main.cpp -o obj/main.o

obj/akinator.o: src/akinator.cpp ${Headers}
	g++ -c src/akinator.cpp -o obj/akinator.o

obj/akinator_dump.o: src/akinator_dump.cpp ${Headers}
	g++ -c src/akinator_dump.cpp -o obj/akinator_dump.o

run:
	./akinator.out

target:
	mkdir log
	touch log/dump.dot
	touch log/dump.html

clean:
	rm -r log
	rm binary_sort_tree.out
