#include "common.h"
#include "dna_seq.h"

long long dna_seq_init_count = 0;
long long dna_seq_realloc_count = 0;

dna_seq_t* init_dna_seq_with_size(size_t size) {
  dna_seq_t* dna_seq = (dna_seq_t*) malloc(sizeof(dna_seq_t));
  dna_seq->start = (char*) malloc(size);
  dna_seq->end = dna_seq->start;
  dna_seq->size = size;
  dna_seq_init_count++;
  return dna_seq;
}

dna_seq_t* init_dna_seq() {
  return init_dna_seq_with_size(DNA_SEQ_INIT_SIZE);
}

dna_seq_t* init_dna_seq_from_str(char* str) {
  dna_seq_t* dna_seq = init_dna_seq_with_size(strlen(str) + 1);
  memcpy(dna_seq->start, str, strlen(str));
  dna_seq->end = dna_seq->start + strlen(str);
  return dna_seq;
}

dna_seq_t* clone_dna_seq(dna_seq_t *dna_seq) {
  dna_seq_t* cloned_dna_seq = init_dna_seq_with_size(dna_seq->size);
  memcpy(cloned_dna_seq->start, dna_seq->start, dna_seq->size);
  cloned_dna_seq->end = cloned_dna_seq->start + (dna_seq->end - dna_seq->start);
  return cloned_dna_seq;
}

void free_dna_seq(dna_seq_t* dna_seq) {
  free(dna_seq->start);
  free(dna_seq);
}

void append_to_dna_seq(dna_seq_t* dna_seq, char c) {
  *(dna_seq->end) = c;
  dna_seq->end++;
  if (dna_seq->end - dna_seq->start == dna_seq->size) {
    dna_seq->start = realloc(dna_seq->start, dna_seq->size * DNA_SEQ_SIZE_FACTOR);
    dna_seq->end = dna_seq->start + dna_seq->size;
    dna_seq->size *= DNA_SEQ_SIZE_FACTOR;
    dna_seq_realloc_count++;
  }
}

int dna_seq_equal(dna_seq_t* one, dna_seq_t* two) {
  if (one->size != two->size) {
    return 0;
  }
  if (one->end - one->start != two->end - two->start) {
    return 0;
  }
  char* one_ptr = one->start;
  char* two_ptr = two->start;
  while (one_ptr != one->end) {
    if (*one_ptr != *two_ptr) {
      return 0;
    }
    one_ptr++;
    two_ptr++;
  }

  return 1;
}
