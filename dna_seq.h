#ifndef DNA_SEQ_H
#define DNA_SEQ_H

#include "common.h"
#include "rna.h"

/* Default initial size of the sequence. */
#define DNA_SEQ_INIT_SIZE (1024)
/* Factor to increase the size by when we reallocate. */
#define DNA_SEQ_SIZE_FACTOR (2)

typedef struct dna_seq_t_ dna_seq_t;

struct dna_seq_t_ {
  /* Start of the internal buffer. */
  char* start;
  /* Current marker, position to read from. */
  char* cur;
  /* End marker, points past the last written data char. */
  char* end;
  /* Next segment in the chain, see dna_chain.c. */
  dna_seq_t* next;
  /* Current size of the internal buffer. */
  size_t size;
};

dna_seq_t* init_dna_seq();
dna_seq_t* init_dna_seq_with_size(size_t size);
dna_seq_t* init_dna_seq_from_ptr(char* str, size_t size);
dna_seq_t* init_dna_seq_from_str(char* str);
dna_seq_t* clone_dna_seq(dna_seq_t* dna_seq);
void free_dna_seq(dna_seq_t* dna_seq);
char consume_base(dna_seq_t* dna_seq);
int nat(dna_seq_t* dna, rna_t* rna);
void append_to_dna_seq(dna_seq_t* dna_seq, char c);
void append_to_dna_seq_from_ptr(dna_seq_t* dna_seq, char* src, size_t size);
char get_from_dna_seq(dna_seq_t* dna_seq, size_t pos);
int dna_seq_equal(dna_seq_t* one, dna_seq_t* two);
int dna_seq_match(char* one, char* two, size_t size);

#endif  /* DNA_SEQ_H */
