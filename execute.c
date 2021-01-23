#include "execute.h"
#include "dna_seq.h"
#include "pattern.h"
#include "template.h"
#include "matchreplace.h"

void execute(char* filename) {
  FILE* fh = fopen(filename, "r");
  assert(fh != NULL);
  /* Determine the file size. */
  fseek(fh, 0, SEEK_END);
  long dna_size = ftell(fh);
  rewind(fh);
  char* dna_buf = (char *) malloc(dna_size);
  assert(fread(dna_buf, 1, dna_size, fh) == dna_size);
  fclose(fh);

  dna_seq_t* dna = init_dna_seq_from_ptr(dna_buf, dna_size);
  free(dna_buf);
  printf("[%s:%d] Initial DNA sequence: %d bases.\n", __FILE__, __LINE__, dna->end - dna->start);
  rna_t rna;
  init_rna(&rna);

  int iter = 0;
  while(1) {
    pitem_seq_t* p = pattern(dna, &rna);
    titem_seq_t* t = template(dna, &rna);
    dna_seq_t* new_dna = matchreplace(dna, p, t);
    printf("matchreplace generated new sequence with size=%d\n", new_dna->end - new_dna->start);
    free_dna_seq(dna);
    dna = new_dna;
    free(p);
    free(t);
    printf("[%s:%d] Completed iteration %d.\n", __FILE__, __LINE__, iter);
    iter++;
    if (iter == 1000) {
      break;
    }
  }
}
