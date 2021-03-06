#include "common.h"
#include "pattern.h"

void pattern_test() {
  dna_seq_t* dna = init_dna_seq_from_str("CIIC");
  pitem_seq_t* out_pattern_seq = pattern(dna, NULL);
  pitem_t* pitem_arr = out_pattern_seq->start;
  assert(pitem_arr[0].type == PITEM_BASE);
  assert(pitem_arr[0].base == 'I');
  assert(out_pattern_seq->end - out_pattern_seq->start == 1);
  free_pattern_seq(out_pattern_seq);
  free_dna_seq(dna);

  dna = init_dna_seq_from_str("IIPIPICPIICICIIF");
  out_pattern_seq = pattern(dna, NULL);
  pitem_arr = out_pattern_seq->start;
  assert(pitem_arr[0].type == PITEM_OPEN_GROUP);
  assert(pitem_arr[1].type == PITEM_SKIP_N);
  assert(pitem_arr[1].skip == 2);
  assert(pitem_arr[2].type == PITEM_CLOSE_GROUP);
  assert(pitem_arr[3].type == PITEM_BASE);
  assert(pitem_arr[3].base == 'P');
  assert(out_pattern_seq->end - out_pattern_seq->start == 4);
  free_pattern_seq(out_pattern_seq);
  free_dna_seq(dna);

  /* Test which exercises PITEM_DNA_SEQ, which allocates memory. */
  /* IF   - trigger for PITEM_DNA_SEQ.                           */
  /* F    - dummy base, consumed after IF.                       */
  /* PPPP - consumed by consts().                                */
  /* IIC  - II acts as a termination marker for consts(), which  */
  /*        is examined (but not consumed!) by it. IIC then acts */
  /*        as a termination marker for pattern().               */
  dna = init_dna_seq_from_str("IFFPPPPIIC");
  out_pattern_seq = pattern(dna, NULL);
  pitem_arr = out_pattern_seq->start;
  assert(pitem_arr[0].type == PITEM_DNA_SEQ);
  *pitem_arr[0].dna_seq->end = '\0';
  assert(strcmp("FFFF", pitem_arr[0].dna_seq->start) == 0);
  free_pattern_seq(out_pattern_seq);
  free_dna_seq(dna);
}

int main(int unused_argc, char** unused_argv) {
  pattern_test();
}
