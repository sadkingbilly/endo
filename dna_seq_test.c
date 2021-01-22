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

void init_dna_seq_from_ptr_test() {
  dna_seq_t* dna_seq = init_dna_seq_from_ptr("ICFP", 2);

  dna_seq_t* expected = init_dna_seq_with_size(2);
  append_to_dna_seq(expected, 'I');
  append_to_dna_seq(expected, 'C');
  expected->size = dna_seq->size;
  assert(dna_seq_equal(dna_seq, expected));
  free_dna_seq(dna_seq);
  free_dna_seq(expected);
}

void init_dna_seq_from_str_test() {
  dna_seq_t* dna_seq = init_dna_seq_from_str("ICFP");

  dna_seq_t* expected = init_dna_seq_with_size(4);
  append_to_dna_seq(expected, 'I');
  append_to_dna_seq(expected, 'C');
  append_to_dna_seq(expected, 'F');
  append_to_dna_seq(expected, 'P');
  expected->size = dna_seq->size;
  assert(dna_seq_equal(dna_seq, expected));
  free_dna_seq(dna_seq);
  free_dna_seq(expected);
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

void get_from_dna_seq_test() {
  dna_seq_t* dna_seq = init_dna_seq_from_str("ICFP");
  assert(get_from_dna_seq(dna_seq, 0) == 'I');
  assert(get_from_dna_seq(dna_seq, 1) == 'C');
  assert(get_from_dna_seq(dna_seq, 2) == 'F');
  assert(get_from_dna_seq(dna_seq, 3) == 'P');
  assert(get_from_dna_seq(dna_seq, 4) == '\0');
  assert(get_from_dna_seq(dna_seq, 32) == '\0');
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

void nat_test() {
  dna_seq_t* dna;

  /* All test sequences must have 'P' in them, otherwise nat() */
  /* will reach the end of the sequence and call finish().     */
  dna = init_dna_seq_from_str("P");
  assert(nat(dna, NULL) == 0);
  free_dna_seq(dna);

  dna = init_dna_seq_from_str("FP");
  assert(nat(dna, NULL) == 0);
  free_dna_seq(dna);

  dna = init_dna_seq_from_str("CP");
  assert(nat(dna, NULL) == 1);
  free_dna_seq(dna);

  dna = init_dna_seq_from_str("FCP");
  assert(nat(dna, NULL) == 2);
  free_dna_seq(dna);

  dna = init_dna_seq_from_str("CFCP");
  assert(nat(dna, NULL) == 5);
  free_dna_seq(dna);
}
 
void dna_seq_match_test() {
  assert(dna_seq_match("ICFP", "ICFC", 3) == 1);
  assert(dna_seq_match("ICFP", "ICCC", 3) == 0);
}

int main(int unused_argc, char** unused_argv) {
  init_dna_seq_test();
  init_dna_seq_from_ptr_test();
  init_dna_seq_from_str_test();
  init_dna_seq_with_size_test();
  dna_seq_append_test();
  get_from_dna_seq_test();
  dna_seq_realloc_test();
  clone_dna_seq_test();
  dna_seq_equal_test();
  nat_test();
}
