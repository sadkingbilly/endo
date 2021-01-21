CC = gcc
CFLAGS := -fsanitize=address -g

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%_test: common.o dna_seq.o %.o %_test.o
	$(CC) $(CFLAGS) $? -o $@

tests: dna_seq_test pattern_test replace_test matchreplace_test
	./dna_seq_test
	./pattern_test
	./replace_test
	./matchreplace_test

clean:
	rm -f *.o *_test

.PHONY: tests clean
