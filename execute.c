#include "execute.h"
#include "dna_seq.h"
#include "pattern.h"
#include "template.h"
#include "matchreplace.h"
#include <time.h>

#ifdef PROFILER
#include <gperftools/profiler.h>
#endif  /* PROFILER */

void execute(char* filename, long max_iters) {
  FILE* fh = fopen(filename, "r");
  assert(fh != NULL);
  /* Determine the file size. */
  fseek(fh, 0, SEEK_END);
  long dna_size = ftell(fh);
  rewind(fh);
  char* dna_buf = (char *) malloc(dna_size);
  size_t read_size = fread(dna_buf, 1, dna_size, fh);
  assert(read_size == dna_size);
  fclose(fh);

  dna_seq_t* dna = init_dna_seq_from_ptr(dna_buf, dna_size);
  free(dna_buf);
  printf("[%s:%d] Initial DNA sequence: %d bases.\n", __FILE__, __LINE__, dna->end - dna->start);
  rna_t rna;
  init_rna(&rna);

  init_asnat_table();

  int iter = 0;
  time_t start_seconds = time(NULL);
  time_t elapsed_seconds;
#ifdef PROFILER
  ProfilerStart("execute.prof");
#endif
  while(iter < max_iters || max_iters == -1) {
    pitem_seq_t* p = pattern(dna, &rna);
    titem_seq_t* t = template(dna, &rna);
    dna_seq_t* new_dna = matchreplace(dna, p, t);
    free_dna_seq(dna);
    dna = new_dna;
    free_pattern_seq(p);
    free_template_seq(t);
    iter++;
    if (iter % 10000 == 0) {
      elapsed_seconds = time(NULL) - start_seconds;
      printf("Completed %d iterations in %d seconds.\n", iter, elapsed_seconds);
    }
  }
#ifdef PROFILER
  ProfilerStop();
  ProfilerFlush();
#endif
}
