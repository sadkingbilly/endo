#ifndef PATTERN_H
#define PATTERN_H

#define MAX_PATTERN_SEQ_SIZE (1024)

typedef enum {
  EMPTY,
  BASE,
  SKIP_N,
  DNA_SEQ,
  OPEN_GROUP,
  CLOSE_GROUP
} pitem_types_t;

typedef struct {
  pitem_types_t type;
  char base;
  int skip;
  void* dna_seq_ptr;
} pitem_t;

typedef pitem_t pattern_seq_t[MAX_PATTERN_SEQ_SIZE];

int process_simple(char** dna_seq_ptr_ptr, pitem_t** pattern_ptr_ptr);
int process_after_i(char** dna_seq_ptr_ptr, pitem_t** pattern_ptr_ptr);
int pattern(char* in_dna_seq, char* out_rna, pattern_seq_t out_pattern);

#endif  // PATTERN_H
