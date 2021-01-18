CC = gcc

%.o : %.c
	$(CC) -c $< -o $@

%_test: common.o %.o %_test.o
	$(CC) $? -o $@

tests: pattern_test replace_test
	./pattern_test
	./replace_test

clean:
	rm -f *.o *_test

.PHONY: tests clean
