Headers := include/akinator.h include/akinator_dump.h include/big_array.h include/definition_mode.h include/difference_mode.h include/guess_mode.h include/node_utils.h include/read_database.h include/save_database.h include/stack_utils.h include/utils.h
Objects := obj/main.o obj/akinator.o obj/akinator_dump.o obj/stack.o obj/big_array.o obj/definition_mode.o obj/difference_mode.o obj/guess_mode.o obj/node_utils.o obj/read_database.o obj/save_database.o obj/stack_utils.o obj/utils.o

akinator.out: ${Objects} ${Headers}
	g++ ${Objects} -o akinator.out

obj/main.o: src/main.cpp ${Headers}
	g++ -c src/main.cpp -o obj/main.o

obj/akinator.o: src/akinator.cpp ${Headers}
	g++ -c src/akinator.cpp -o obj/akinator.o

obj/akinator_dump.o: src/akinator_dump.cpp ${Headers}
	g++ -c src/akinator_dump.cpp -o obj/akinator_dump.o

obj/big_array.o: src/big_array.cpp ${Headers}
	g++ -c src/big_array.cpp -o obj/big_array.o

obj/definition_mode.o: src/definition_mode.cpp ${Headers}
	g++ -c src/definition_mode.cpp -o obj/definition_mode.o

obj/difference_mode.o: src/difference_mode.cpp ${Headers}
	g++ -c src/difference_mode.cpp -o obj/difference_mode.o

obj/guess_mode.o: src/guess_mode.cpp ${Headers}
	g++ -c src/guess_mode.cpp -o obj/guess_mode.o

obj/node_utils.o: src/node_utils.cpp ${Headers}
	g++ -c src/node_utils.cpp -o obj/node_utils.o

obj/read_database.o: src/read_database.cpp ${Headers}
	g++ -c src/read_database.cpp -o obj/read_database.o

obj/save_database.o: src/save_database.cpp ${Headers}
	g++ -c src/save_database.cpp -o obj/save_database.o

obj/stack_utils.o: src/stack_utils.cpp ${Headers}
	g++ -c src/stack_utils.cpp -o obj/stack_utils.o

obj/utils.o: src/utils.cpp ${Headers}
	g++ -c src/utils.cpp -o obj/utils.o

obj/stack.o: lib/stack/stack_hcpp/stack.cpp ${Headers}
	g++ -c lib/stack/stack_hcpp/stack.cpp -o obj/stack.o

run:
	./akinator.out

target:
	mkdir log
	touch log/dump.dot
	touch log/dump.html

clean:
	rm -r log
	rm binary_sort_tree.out
