#include "common.h"
#include "rna.h"

void init_rna_test() {
  rna_t rna;
  init_rna(&rna);
  assert(rna.end == rna.data);
}

void emit_rna_test() {
  rna_t rna;
  emit_rna(&rna, "ICFP", 4);
  emit_rna(&rna, "IC", 2);
  assert(strcmp(rna.data[0], "ICFP") == 0);
  assert(strcmp(rna.data[1], "IC") == 0);
  assert(rna.end - rna.data == 2);
  free_rna_data(&rna);
}

int main(int unused_argc, char** unused_argv) {
  init_rna_test();
  emit_rna_test();
}
