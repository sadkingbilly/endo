#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "pattern.h"

char consume(char **ptr_ptr) {
  char c = **ptr_ptr;
  if (c != '\0') {
    (*ptr_ptr)++;
  }
  return c;
}

int nat(char** dna_seq_ptr_ptr) {
  int n;

  char base = consume(dna_seq_ptr_ptr);
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

void consts(char** dna_seq_ptr_ptr, char* out_buf, size_t out_buf_size) {
  char base = **dna_seq_ptr_ptr;
  char next_base = *(*dna_seq_ptr_ptr + 1);

  assert(out_buf_size > 0);

  if (base == 'C') {
    consume(dna_seq_ptr_ptr);
    out_buf[0] = 'I';
    consts(dna_seq_ptr_ptr, out_buf + 1, out_buf_size - 1);
  }
  if (base == 'F') {
    consume(dna_seq_ptr_ptr);
    out_buf[0] = 'C';
    consts(dna_seq_ptr_ptr, out_buf + 1, out_buf_size - 1);
  }
  if (base == 'P') {
    consume(dna_seq_ptr_ptr);
    out_buf[0] = 'F';
    consts(dna_seq_ptr_ptr, out_buf + 1, out_buf_size - 1);
  }
  if (base == 'I' && next_base == 'C') {
    consume(dna_seq_ptr_ptr);
    consume(dna_seq_ptr_ptr);
    out_buf[0] = 'P';
    consts(dna_seq_ptr_ptr, out_buf + 1, out_buf_size - 1);
  }

  out_buf[0] = '\0';
}

#define CONSTS_BUF_SIZE (1024)

int pattern(char* in_dna_seq, char* out_rna, pattern_seq_t out_pattern) {
  pitem_t* pattern_ptr = out_pattern;
  int lvl = 0;

  while (1) {
    char selector[4] = {consume(&in_dna_seq), '\0', '\0', '\0'};
    if (selector[0] == 'I') {
      selector[1] = consume(&in_dna_seq);
      if (selector[1] == 'I') {
        selector[2] = consume(&in_dna_seq);
      }
    }

    if (strcmp(selector, "C") == 0) {
      *pattern_ptr++ = (pitem_t) {.type = BASE, .base = 'I'};
      continue;
    }
    if (strcmp(selector, "F") == 0) {
      *pattern_ptr++ = (pitem_t) {.type = BASE, .base = 'C'};
      continue;
    }
    if (strcmp(selector, "P") == 0) {
      *pattern_ptr++ = (pitem_t) {.type = BASE, .base = 'F'};
      continue;
    }
    if (strcmp(selector, "IC") == 0) {
      *pattern_ptr++ = (pitem_t) {.type = BASE, .base = 'P'};
      continue;
    }
    if (strcmp(selector, "IP") == 0) {
      int n = nat(&in_dna_seq);
      if (n == STATUS_FINISH) {
        break;
      }
      *pattern_ptr++ = (pitem_t) {.type = SKIP_N, .skip = n};
      continue;
    }
    if (strcmp(selector, "IF") == 0) {
      /* Special consumption of extra base (unused) for 'IF' case. */
      consume(&in_dna_seq);
      /* Fixed size buffer for now. consts() has overrun detection, so this */
      /* may be optimized, if it causes issues.                             */
      char* consts_buf = (char *) malloc(CONSTS_BUF_SIZE);
      consts(&in_dna_seq, consts_buf, CONSTS_BUF_SIZE);
      *pattern_ptr++ = (pitem_t) {.type = DNA_SEQ, .dna_seq_ptr = consts_buf};
      continue;
    }
    if (strcmp(selector, "IIP") == 0) {
      lvl++;
      *pattern_ptr++ = (pitem_t) {.type = OPEN_GROUP};
      continue;
    }
    if (strcmp(selector, "IIC") == 0 || strcmp(selector, "IIF") == 0) {
      if (lvl == 0) {
        break;
      }
      lvl--;
      *pattern_ptr++ = (pitem_t) {.type = CLOSE_GROUP};
      continue;
    }
    if (strcmp(selector, "III") == 0) {
      for (int i = 0; i < 7; i++) {
        *out_rna++ = consume(&in_dna_seq);
      }
      continue;
    }

    /* Did not match any selectors, terminating. */
    break;
  }

  return STATUS_OK;
}
