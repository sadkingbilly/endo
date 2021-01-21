#ifndef DNA_SEQ_H
#define DNA_SEQ_H

#include <stddef.h>

/* Default initial size of the sequence. */
#define DNA_SEQ_INIT_SIZE (32)
/* Factor to increase the size by when we reallocate. */
#define DNA_SEQ_SIZE_FACTOR (2)

typedef struct {
  /* Start of the internal buffer. */
  char* start;
  /* Current marker, position to read from. */
  char* cur;
  /* End marker, points past the last written data char. */
  char* end;
  /* Current size of the internal buffer. */
  size_t size;
} dna_seq_t;

dna_seq_t* init_dna_seq();
dna_seq_t* init_dna_seq_with_size(size_t size);
dna_seq_t* init_dna_seq_from_str(char* str);
dna_seq_t* clone_dna_seq(dna_seq_t* dna_seq);
void free_dna_seq(dna_seq_t* dna_seq);
char consume_base(dna_seq_t* dna_seq);
int nat(dna_seq_t* dna);
void append_to_dna_seq(dna_seq_t* dna_seq, char c);
char get_from_dna_seq(dna_seq_t* dna_seq, size_t pos);
int dna_seq_equal(dna_seq_t* one, dna_seq_t* two);

#endif  /* DNA_SEQ_H */
