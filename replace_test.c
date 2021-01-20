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

  quoted = protect(2, orig);
  expected = init_dna_seq_from_str("FPICCF");
  expected->size = quoted->size;
  assert(dna_seq_equal(expected, quoted));
  free_dna_seq(quoted);
}

void replace_test() {
}

int main(int unused_argc, char** unused_argv) {
  protect_test();
  replace_test();
}
