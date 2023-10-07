btree:
	g++ -c src/bplustree_utils.cpp -I include/

buffer:
	g++ -c src/buffer_pool.cpp -I include/

install: btree buffer
	g++ -o dbfile src/main.cpp bplustree_utils.o buffer_pool.o -I include/

clean:
	rm -rf dbfile
	rm -rf *.o
