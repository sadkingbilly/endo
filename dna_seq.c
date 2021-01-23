#include "common.h"
#include "dna_seq.h"

long long dna_seq_init_count = 0;
long long dna_seq_realloc_count = 0;

dna_seq_t* init_dna_seq_with_size(size_t size) {
  dna_seq_t* dna_seq = (dna_seq_t*) malloc(sizeof(dna_seq_t));
  dna_seq->start = (char*) malloc(size);
  dna_seq->cur = dna_seq->start;
  dna_seq->end = dna_seq->start;
  dna_seq->size = size;
  dna_seq_init_count++;
  return dna_seq;
}

dna_seq_t* init_dna_seq() {
  return init_dna_seq_with_size(DNA_SEQ_INIT_SIZE);
}

dna_seq_t* init_dna_seq_from_ptr(char* str, size_t size) {
  /* Extra one byte, so that end points within the buffer. */
  dna_seq_t* dna_seq = init_dna_seq_with_size(size + 1);
  memcpy(dna_seq->start, str, size);
  dna_seq->end = dna_seq->start + size;
  return dna_seq;
}

dna_seq_t* init_dna_seq_from_str(char* str) {
  return init_dna_seq_from_ptr(str, strlen(str));
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

/* Reads from and advances dna->cur. */
char consume_base(dna_seq_t* dna_seq) {
  if (dna_seq->cur == dna_seq->end) {
      return '\0';
  }
  char base = *dna_seq->cur;
  dna_seq->cur++;
  return base;
}

/* Reads from and advances dna_seq->cur. */
int nat(dna_seq_t* dna, rna_t* rna) {
  int n;
  char base = consume_base(dna);

  switch (base) {
    case 'P':
      return 0;
    case 'I':
    case 'F':
      return 2 * nat(dna, rna);
    case 'C':
      return 2 * nat(dna, rna) + 1;
  }

  /* finish() does not return. */
  finish(rna);
  /* Not reached. */
  return -1;
}

void append_to_dna_seq(dna_seq_t* dna_seq, char c) {
  *(dna_seq->end) = c;
  dna_seq->end++;
  if (dna_seq->end - dna_seq->start == dna_seq->size) {
    size_t cur_offset = dna_seq->cur - dna_seq->start;
    size_t new_size = dna_seq->size * DNA_SEQ_SIZE_FACTOR;
    dna_seq->start = realloc(dna_seq->start, new_size);
    dna_seq->cur = dna_seq->start + cur_offset;
    dna_seq->end = dna_seq->start + dna_seq->size;
    dna_seq->size = new_size;
    dna_seq_realloc_count++;
  }
}

char get_from_dna_seq(dna_seq_t* dna_seq, size_t pos) {
  if (dna_seq->start + pos < dna_seq->end) {
    return dna_seq->start[pos];
  } else {
    return '\0';
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

int dna_seq_match(char* one, char* two, size_t size) {
  for (int i = 0; i < size; i++) {
    if (one[i] != two[i]) {
      return 0;
    }
  }
  return 1;
}
