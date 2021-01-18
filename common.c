#include "common.h"

char consume_base(char **dna_seq_ptr_ptr) {
  char base = **dna_seq_ptr_ptr;
	/* Do not advance past the end of the sequence. */
  if (base != '\0') {
    (*dna_seq_ptr_ptr)++;
  }
  return base;
}

int nat(char** dna_seq_ptr_ptr) {
  int n;
  char base = consume_base(dna_seq_ptr_ptr);

  switch (base) {
    case 'P':
      return 0;
    case 'I':
    case 'F':
      n = nat(dna_seq_ptr_ptr);
      return n == STATUS_FINISH ? STATUS_FINISH : 2 * n;
    case 'C':
      n = nat(dna_seq_ptr_ptr);
      return n == STATUS_FINISH ? STATUS_FINISH : 2 * n + 1;
  }

  /* Can only be a termination character. */
  assert(base == '\0');
  return STATUS_FINISH;
}
