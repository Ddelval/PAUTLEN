.PHONY: clean

CC=g++-11
FLAGS= -g 

test: ht.o main.o
	$(CC) $(FLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(FLAGS) -c $^

clean:
	rm *.o