#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "dna_seq.h"

#define TEMPLATE_SIZE (1024)

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

titem_seq_t* init_template_seq();
void emit_titem(titem_seq_t* out_seq, titem_t item);
titem_seq_t* template(dna_seq_t* dna, char* out_rna);

#endif  /* TEMPLATE_H */
