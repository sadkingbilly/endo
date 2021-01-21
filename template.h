#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "dna_seq.h"

typedef enum {
  TITEM_BASE,
  TITEM_REF,
  TITEM_LEN,
} titem_types_t;

typedef struct {
  titem_types_t type;
  /* For TITEM_BASE. */
  char base;
  /* For TITEM_REF. */
  int ref_num;
  int prot_level;
  /* For TITEM_LEN. */
  int len;
} titem_t;

typedef struct {
  titem_t* start;
  titem_t* next;
  size_t size;
} titem_seq_t;

titem_seq_t* init_template_seq(size_t size);
void emit_titem(titem_seq_t* out_seq, titem_t item);
int template(dna_seq_t* dna, char* out_rna, titem_seq_t* out_template_seq);

#endif  /* TEMPLATE_H */
