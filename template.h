#ifndef TEMPLATE_H
#define TEMPLATE_H

typedef enum {
  TITEM_EMPTY,
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
int template(char* in_dna_seq, char* out_rna, titem_seq_t* out_template_seq);

#endif  /* TEMPLATE_H */
