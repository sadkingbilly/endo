#include "replace.h"
#include "common.h"

void protect_test() {
  dna_seq_t* quoted;
  dna_seq_t* expected;
  dna_seq_t* orig = init_dna_seq_from_str("ICFP");

  quoted = protect(0, orig);
  assert(dna_seq_equal(orig, quoted));
  free_dna_seq(quoted);

  quoted = protect(1, orig);
  expected = init_dna_seq_from_str("CFPIC");
  expected->size = quoted->size;
  assert(dna_seq_equal(expected, quoted));
  free_dna_seq(quoted);
  free_dna_seq(expected);

  quoted = protect(2, orig);
  expected = init_dna_seq_from_str("FPICCF");
  expected->size = quoted->size;
  assert(dna_seq_equal(expected, quoted));
  free_dna_seq(quoted);
  free_dna_seq(expected);

  free_dna_seq(orig);
}

void asnat_test() {
  dna_seq_t* seq = asnat(0);
  seq->end = '\0';
  assert(strcmp("P", seq->start));
  free_dna_seq(seq);

  seq = asnat(1);
  seq->end = '\0';
  assert(strcmp("CP", seq->start));
  free_dna_seq(seq);

  seq = asnat(2);
  seq->end = '\0';
  assert(strcmp("ICP", seq->start));
  free_dna_seq(seq);

  seq = asnat(8);
  seq->end = '\0';
  assert(strcmp("IIICP", seq->start));
  free_dna_seq(seq);

  seq = asnat(17);
  seq->end = '\0';
  assert(strcmp("CIIICP", seq->start));
  free_dna_seq(seq);
}

void replace_test() {
}

int main(int unused_argc, char** unused_argv) {
  protect_test();
  asnat_test();
  replace_test();
}
