#include "common.h"
#include "matchreplace.h"

void matchreplace_base_test(char* input, char* expected) {
  dna_seq_t* dna;
  dna_seq_t* new_dna;
  dna_seq_t* expected_dna;
  pitem_seq_t* p;
  titem_seq_t* t;
  rna_t rna;
  
  dna = init_dna_seq_from_str(input);
  p = pattern(dna, &rna);
  t = template(dna, &rna);
  new_dna = matchreplace(dna, p, t);
  expected_dna = init_dna_seq_from_str(expected);
  expected_dna->size = new_dna->size;
  assert(dna_seq_equal(expected_dna, new_dna));

  free_dna_seq(dna);
  free_dna_seq(new_dna);
  free_dna_seq(expected_dna);
  free_pattern_seq(p);
  free_template_seq(t);
}

void matchreplace_test() {
  // matchreplace_base_test("IIPIPICPIICICIIFICCIFPPIICCFPC", "PICFC");
  // matchreplace_base_test("IIPIPICPIICICIIFICCIFCCCPPIICCFPC", "PIICCFCFFPC");
  matchreplace_base_test("IIPIPIICPIICIICCIICFCFC", "I");
}

int main(int unused_argc, char** unused_argv) {
  matchreplace_test();
}
