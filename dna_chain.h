#ifndef DNA_CHAIN_H
#define DNA_CHAIN_H

#include "common.h"
#include "dna_seq.h"

/* Describes a linked list of DNA sequences. Processing always starts from   */
/* the first sequence, and if we get to the end of it, we remove it from the */
/* chain, free it, and advance the start pointer. Thus, the pointer to the   */
/* current base can always be accessed as dna_chain->start->cur.             */
typedef struct {
  dna_seq_t* start;
  dna_seq_t* end;
} dna_chain_t;

void prepend_dna(dna_chain_t* chain, dna_seq_t* dna);
void append_dna(dna_chain_t* chain, dna_seq_t* dna);
char next_in_chain(dna_chain_t* chain);
char consume_from_chain(dna_chain_t* chain);

#endif  /* DNA_CHAIN_H */

