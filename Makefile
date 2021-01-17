CC = gcc

%.o : %.c
	$(CC) -c $< -o $@

%_test: %.o %_test.o
	$(CC) $? -o $@

clean:
	rm *.o
