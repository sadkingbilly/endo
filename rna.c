#include "common.h"
#include "rna.h"

void init_rna(rna_t* rna) {
  rna->end = rna->data;
}

void free_rna_data(rna_t* rna) {
  char** ptr = rna->data;
  while (ptr != rna->end) {
    free (*ptr);
    ptr++;
  }
}

/* Outputs RNA and terminates the program. */
void finish(rna_t* rna) {
  char** ptr = rna->data;
  while (ptr != rna->end) {
    printf("[%d] %s\n", ptr - rna->data, *ptr);
    ptr++;
  }
  exit(0);
}

void emit_rna(rna_t* rna, char* src, size_t size) {
  *rna->end = (char*) malloc(size + 1);
  memcpy(*rna->end, src, size);
  *(*rna->end + size) = '\0';
  rna->end++;
  assert(rna->end - rna->data < RNA_SIZE);
}
