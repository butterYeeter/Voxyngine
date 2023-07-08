build/main: main.o
	gcc -L./lib -Wl,-rpath=/home/raaid/Documents/c/opengl/build -o $@ main.o -lglad -lglfw
	cp lib/* build/
	cp -r resources build/
	rm *.o

main.o: src/main.c
	gcc -L./lib -I./include -Wl,-rpath,./ -c src/main.c -lglad -lglfw

help:
	@echo "List of commands"
	@echo "\tbuild/main: builds the main executable"
	@echo "\tsrc/main.o: builds the main.o object file needed for the main target"
	@echo "\trun: runs main"