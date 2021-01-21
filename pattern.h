#ifndef PATTERN_H
#define PATTERN_H

#include "common.h"
#include "dna_seq.h"

typedef enum {
  PITEM_BASE,
  PITEM_SKIP_N,
  PITEM_DNA_SEQ,
  PITEM_OPEN_GROUP,
  PITEM_CLOSE_GROUP
} pitem_types_t;

typedef struct {
  pitem_types_t type;
  /* For PITEM_BASE. */
  char base;
  /* For PITEM_SKIP_N. */
  int skip;
  /* For PITEM_DNA_SEQ. */
  dna_seq_t* dna_seq;
} pitem_t;

typedef struct {
  pitem_t* start;
  pitem_t* end;
  size_t size;
} pitem_seq_t;

pitem_seq_t* init_pattern_seq(size_t size);
void free_pattern_seq(pitem_seq_t* pitem_seq);
void emit_pitem(pitem_seq_t* out_seq, pitem_t item);
int pattern(dna_seq_t* dna, char* out_rna, pitem_seq_t* out_pattern_seq);

#endif  /* PATTERN_H */
