#include "common.h"
#include "dna_seq.h"

void init_dna_seq_test() {
  dna_seq_t* dna_seq = init_dna_seq();
  assert(dna_seq->size == DNA_SEQ_INIT_SIZE);
  assert(dna_seq->start == dna_seq->end);
}

void init_dna_seq_with_size_test() {
  dna_seq_t* dna_seq = init_dna_seq_with_size(64);
  assert(dna_seq->size == 64);
  assert(dna_seq->start == dna_seq->end);
}

void dna_seq_append_test() {
  dna_seq_t* dna_seq = init_dna_seq();
  assert(dna_seq->size == DNA_SEQ_INIT_SIZE);
  assert(dna_seq->start == dna_seq->end);

  append_to_dna_seq(dna_seq, 'I');
  assert(dna_seq->end - dna_seq->start == 1);
  append_to_dna_seq(dna_seq, 'C');
  assert(dna_seq->end - dna_seq->start == 2);
  append_to_dna_seq(dna_seq, 'F');
  assert(dna_seq->end - dna_seq->start == 3);
  append_to_dna_seq(dna_seq, 'P');
  assert(dna_seq->end - dna_seq->start == 4);

  append_to_dna_seq(dna_seq, '\0');
  assert(strcmp(dna_seq->start, "ICFP") == 0);
}

void dna_seq_realloc_test() {
  dna_seq_t* dna_seq = init_dna_seq();
  for (int i = 0; i < DNA_SEQ_INIT_SIZE + 4; i++) {
    append_to_dna_seq(dna_seq, 'I');
  }
  /* Overflowing the original size should trigger realloc. */
  assert(dna_seq->size == DNA_SEQ_SIZE_FACTOR * DNA_SEQ_INIT_SIZE);
  assert(dna_seq->end - dna_seq->start == DNA_SEQ_INIT_SIZE + 4);
}

void clone_dna_seq_test() {
  dna_seq_t* dna_seq = init_dna_seq();
  for (int i = 0; i < 10; i++) {
    append_to_dna_seq(dna_seq, 'I');
  }
  dna_seq_t* cloned_dna_seq = clone_dna_seq(dna_seq);
  assert(cloned_dna_seq->size == DNA_SEQ_INIT_SIZE);
  assert(cloned_dna_seq->end - cloned_dna_seq->start == 10);
}

int main(int unused_argc, char** unused_argv) {
  init_dna_seq_test();
  init_dna_seq_with_size_test();
  dna_seq_append_test();
  dna_seq_realloc_test();
  clone_dna_seq_test();
}
