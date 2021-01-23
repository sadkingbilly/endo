CC := gcc
CFLAGS := -g -pg
#CFLAGS := -fsanitize=address -g -pg
#CFLAGS := -O2
DEPS := execute.o matchreplace.o pattern.o replace.o template.o dna_seq.o rna.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%_test: $(DEPS) %.o %_test.o
	$(CC) $(CFLAGS) $? -o $@

tests: dna_seq_test pattern_test template_test replace_test matchreplace_test rna_test execute_test
	./dna_seq_test
	./pattern_test
	./template_test
	./replace_test
	./matchreplace_test
	./rna_test
	./execute_test

run: $(DEPS) run.o
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm -f *.o *_test

.PHONY: tests clean
