#include "common.h"
#include "dna_seq.h"

void init_dna_seq_test() {
  dna_seq_t* dna_seq = init_dna_seq();
  assert(dna_seq->size == DNA_SEQ_INIT_SIZE);
  assert(dna_seq->start == dna_seq->end);
  free_dna_seq(dna_seq);
}

void init_dna_seq_with_size_test() {
  dna_seq_t* dna_seq = init_dna_seq_with_size(64);
  assert(dna_seq->size == 64);
  assert(dna_seq->start == dna_seq->end);
  free_dna_seq(dna_seq);
}

void init_dna_seq_from_str_test() {
  dna_seq_t* dna_seq = init_dna_seq_from_str("ICFP");

  dna_seq_t* expected = init_dna_seq_with_size(5);
  append_to_dna_seq(dna_seq, 'I');
  append_to_dna_seq(dna_seq, 'C');
  append_to_dna_seq(dna_seq, 'F');
  append_to_dna_seq(dna_seq, 'P');

  assert(dna_seq_equal(dna_seq, expected));
  free_dna_seq(dna_seq);
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
  free_dna_seq(dna_seq);
}

void dna_seq_realloc_test() {
  dna_seq_t* dna_seq = init_dna_seq();
  for (int i = 0; i < DNA_SEQ_INIT_SIZE + 4; i++) {
    append_to_dna_seq(dna_seq, 'I');
  }
  /* Overflowing the original size should trigger realloc. */
  assert(dna_seq->size == DNA_SEQ_SIZE_FACTOR * DNA_SEQ_INIT_SIZE);
  assert(dna_seq->end - dna_seq->start == DNA_SEQ_INIT_SIZE + 4);

  free_dna_seq(dna_seq);
}

void clone_dna_seq_test() {
  dna_seq_t* dna_seq = init_dna_seq();
  for (int i = 0; i < 10; i++) {
    append_to_dna_seq(dna_seq, 'I');
  }
  dna_seq_t* cloned_dna_seq = clone_dna_seq(dna_seq);
  assert(cloned_dna_seq->size == DNA_SEQ_INIT_SIZE);
  assert(cloned_dna_seq->end - cloned_dna_seq->start == 10);

  free_dna_seq(cloned_dna_seq);
  free_dna_seq(dna_seq);
}

void dna_seq_equal_test() {
  dna_seq_t* one = init_dna_seq();
  for (int i = 0; i < 10; i++) {
    append_to_dna_seq(one, 'I');
  }

  dna_seq_t *two = clone_dna_seq(one);
  assert(dna_seq_equal(one, two));
  free_dna_seq(two);

  two = clone_dna_seq(one);
  two->size++;
  assert(!dna_seq_equal(one, two));
  free_dna_seq(two);

  two = clone_dna_seq(one);
  two->end++;
  assert(!dna_seq_equal(one, two));
  free_dna_seq(two);

  two = clone_dna_seq(one);
  two->start[3] = 'P';
  assert(!dna_seq_equal(one, two));
  free_dna_seq(two);

  free_dna_seq(one);
}
  

int main(int unused_argc, char** unused_argv) {
  init_dna_seq_test();
  init_dna_seq_with_size_test();
  dna_seq_append_test();
  dna_seq_realloc_test();
  clone_dna_seq_test();
  dna_seq_equal_test();
}
