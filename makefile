Source : sqlite3.o Source_lin.o
	gcc -lstdc++ -o Source Source_lin.o sqlite3.o -lpthread -ldl

Source_lin.o : Source_lin.cpp
	gcc -c Source_lin.cpp
sqlite3.o: sqlite3.c
	gcc -c sqlite3.c

.PHONY:clean
clean:
	rm -f Source sqlite3-demo.* *.o
