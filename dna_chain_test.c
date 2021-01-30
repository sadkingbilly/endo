#include "common.h"
#include "dna_chain.h"

void prepend_dna_test() {
  dna_seq_t* dna_one = init_dna_seq_from_str("IC");
  dna_seq_t* dna_two = init_dna_seq_from_str("FP");

  dna_chain_t chain;
  chain.start = dna_one;
  chain.end = dna_one;
  prepend_dna(&chain, dna_two);
  assert(chain.start == dna_two);
  assert(dna_two->next == dna_one);

  free_dna_seq(dna_one);
  free_dna_seq(dna_two);
}

void append_dna_test() {
  dna_seq_t* dna_one = init_dna_seq_from_str("IC");
  dna_seq_t* dna_two = init_dna_seq_from_str("FP");

  dna_chain_t chain;
  chain.start = dna_one;
  chain.end = dna_one;
  append_dna(&chain, dna_two);
  assert(chain.end == dna_two);
  assert(dna_one->next == dna_two);

  free_dna_seq(dna_one);
  free_dna_seq(dna_two);
}

void next_in_chain_test() {
  dna_chain_t chain;

  /* Simple one-segment chain test. */
  dna_seq_t* dna = init_dna_seq_from_str("ICFP");
  dna->cur++;  /* Now points to 'C'. */
  chain.start = dna;
  char* old_cur = chain.start->cur;
  assert(next_in_chain(&chain) == 'F');
  assert(chain.start->cur == old_cur);
  free_dna_seq(dna);

  /* Two-segment chain test. */
  dna_seq_t* dna_one = init_dna_seq_from_str("IC");
  dna_one->cur++;  /* Now points to 'C'. */
  dna_seq_t* dna_two = init_dna_seq_from_str("FP");
  chain.start = dna_one;
  chain.end = dna_one;
  append_dna(&chain, dna_two);
  old_cur = chain.start->cur;
  assert(next_in_chain(&chain) == 'F');
  /* cur should not advance. */
  assert(chain.start->cur == old_cur);
  free_dna_seq(dna_one);
  free_dna_seq(dna_two);
}

void consume_from_chain_test() {
  dna_chain_t chain;

  /* Simple one-segment chain test. */
  dna_seq_t* dna = init_dna_seq_from_str("ICFP");
  dna->cur++;  /* Now points to 'C'. */
  chain.start = dna;
  char* old_cur = chain.start->cur;
  assert(consume_from_chain(&chain) == 'C');
  assert(chain.start->cur == old_cur + 1);
  free_dna_seq(dna);

  /* Two-segment chain test. */
  dna_seq_t* dna_one = init_dna_seq_from_str("IC");
  dna_one->cur++;  /* Now points to 'C'. */
  dna_seq_t* dna_two = init_dna_seq_from_str("FP");
  chain.start = dna_one;
  chain.end = dna_one;
  append_dna(&chain, dna_two);
  old_cur = chain.start->cur;
  assert(consume_from_chain(&chain) == 'C');
  /* cur should advance to cur of next segment. */
  assert(chain.start->cur == dna_two->cur);
  assert(*chain.start->cur == 'F');
  free_dna_seq(dna_one);
  free_dna_seq(dna_two);
}

void combined_consume_next_test() {
  dna_chain_t chain;

  dna_seq_t* dna_one = init_dna_seq_from_str("IC");
  dna_seq_t* dna_two = init_dna_seq_from_str("FP");
  dna_seq_t* dna_three = init_dna_seq_from_str("IF");
  chain.start = dna_one;
  chain.end = dna_one;

  append_dna(&chain, dna_two);
  append_dna(&chain, dna_three);

  assert(next_in_chain(&chain) == 'C');
  assert(consume_from_chain(&chain) == 'I');
  assert(next_in_chain(&chain) == 'F');
  assert(consume_from_chain(&chain) == 'C');
  assert(chain.start == dna_two);
  assert(next_in_chain(&chain) == 'P');
  assert(consume_from_chain(&chain) == 'F');
  assert(next_in_chain(&chain) == 'I');
  assert(consume_from_chain(&chain) == 'P');
  assert(chain.start == dna_three);
  assert(next_in_chain(&chain) == 'F');
  assert(consume_from_chain(&chain) == 'I');
  assert(consume_from_chain(&chain) == 'F');

  /* No need to free anything, sequences are getting freed by */
  /* consume_from_chain once their end is reached.            */
}

int main(int unused_argc, char** unused_argv) {
  prepend_dna_test();
  append_dna_test();
  next_in_chain_test();
}
