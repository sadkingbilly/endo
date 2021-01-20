CC = gcc

%.o : %.c
	$(CC) -c $< -o $@

%_test: common.o dna_seq.o %.o %_test.o
	$(CC) $? -o $@

tests: pattern_test replace_test dna_seq_test
	./pattern_test
	./replace_test
	./dna_seq_test

clean:
	rm -f *.o *_test

.PHONY: tests clean
