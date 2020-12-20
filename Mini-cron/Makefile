OBJ = main.o funkcje.o
all: hello
hello: $(OBJ)
	gcc $(OBJ) -o minicron
$(OBJ): funkcje.h
.PHONY: clean
clean:
	rm -f *.o minicron
