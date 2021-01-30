#include "common.h"
#include "dna_chain.h"

void prepend_dna(dna_chain_t* chain, dna_seq_t* dna) {
  assert(dna->next == NULL);
  dna->next = chain->start;
  chain->start = dna;
}

void append_dna(dna_chain_t* chain, dna_seq_t* dna) {
  assert(dna->next == NULL);
  assert(chain->end->next == NULL);
  chain->end->next = dna;
  chain->end = dna;
}

/* Returns next base after the current one, cur is not advanced. */
char next_in_chain(dna_chain_t* chain) {
  char* next = chain->start->cur + 1;
  if (next == chain->start->end) {
    /* Reached the end of the current segment, switch to the next one. */
    next = chain->start->next->start;
  }
  return *next;
}

/* Consumes a base from chain, advances cur. */
char consume_from_chain(dna_chain_t* chain) {
  char ret = *(chain->start->cur);
  chain->start->cur++;

  if (chain->start->cur == chain->start->end) {
    /* Reached the end of the current segment, switch to the next one, */
    /* free the finished segment.                                      */
    dna_seq_t* to_free = chain->start;
    chain->start = chain->start->next;
    free_dna_seq(to_free);
  }

  return ret;
}
