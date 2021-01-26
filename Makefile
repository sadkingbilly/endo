CC := gcc
DEPS := execute.o matchreplace.o pattern.o replace.o template.o dna_seq.o rna.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%_test: $(DEPS) %.o %_test.o
	$(CC) $(CFLAGS) $? -o $@

tests: CFLAGS = -fsanitize=address -g
tests: clean dna_seq_test pattern_test template_test replace_test matchreplace_test rna_test execute_test
	./dna_seq_test
	./pattern_test
	./template_test
	./replace_test
	./matchreplace_test
	./rna_test
	./execute_test

run_opt: CFLAGS = -O2
run_opt: $(DEPS) run.o
	$(CC) $(CFLAGS) $? -o $@

run_tcmalloc: CFLAGS = -O2 -ltcmalloc_minimal
run_tcmalloc: $(DEPS) run.o
	$(CC) $(CFLAGS) $? -o $@

run_prof: CFLAGS = -g -DPROFILER
run_prof: LDFLAGS = -lprofiler
run_prof: $(DEPS) run.o
	$(CC) $(CFLAGS) $? $(LDFLAGS) -o $@

clean:
	rm -f *.o *_test run_* 1 Ą gmon.out out.run data/out.run

.PHONY: tests clean
