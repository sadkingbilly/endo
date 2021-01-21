CC = gcc
CFLAGS := -fsanitize=address -g

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%_test: dna_seq.o rna.o %.o %_test.o
	$(CC) $(CFLAGS) $? -o $@

tests: dna_seq_test pattern_test template_test replace_test matchreplace_test rna_test
	./dna_seq_test
	./pattern_test
	./template_test
	./replace_test
	./matchreplace_test
	./rna_test

clean:
	rm -f *.o *_test

.PHONY: tests clean
