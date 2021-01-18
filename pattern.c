#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "pattern.h"

#define CONSTS_BUF_SIZE (1024)

void emit_pitem(pitem_seq_t* out_seq, pitem_t item) {
  assert(out_seq->size);
  assert((out_seq->next - out_seq->start) < out_seq->size);
  *(out_seq->next) = item;
  out_seq->next++;
}

pitem_seq_t* init_pattern_seq(size_t size) {
  pitem_seq_t* seq = (pitem_seq_t*) malloc(sizeof(pitem_seq_t));
  seq->start = (pitem_t*) malloc(size * sizeof(pitem_t));
  seq->next = seq->start;
  seq->size = size;
  return seq;
}

void consts(char** dna_seq_ptr_ptr, char* out_buf, size_t out_buf_size) {
  char base = **dna_seq_ptr_ptr;
  char next_base = *(*dna_seq_ptr_ptr + 1);

  assert(out_buf_size > 0);

  if (base == 'C') {
    consume_base(dna_seq_ptr_ptr);
    out_buf[0] = 'I';
    consts(dna_seq_ptr_ptr, out_buf + 1, out_buf_size - 1);
  }
  if (base == 'F') {
    consume_base(dna_seq_ptr_ptr);
    out_buf[0] = 'C';
    consts(dna_seq_ptr_ptr, out_buf + 1, out_buf_size - 1);
  }
  if (base == 'P') {
    consume_base(dna_seq_ptr_ptr);
    out_buf[0] = 'F';
    consts(dna_seq_ptr_ptr, out_buf + 1, out_buf_size - 1);
  }
  if (base == 'I' && next_base == 'C') {
    consume_base(dna_seq_ptr_ptr);
    consume_base(dna_seq_ptr_ptr);
    out_buf[0] = 'P';
    consts(dna_seq_ptr_ptr, out_buf + 1, out_buf_size - 1);
  }

  out_buf[0] = '\0';
}

int pattern(char* in_dna_seq, char* out_rna, pitem_seq_t* out_pattern_seq) {
  int lvl = 0;

  while (1) {
    char selector[4] = {consume_base(&in_dna_seq), '\0', '\0', '\0'};
    if (selector[0] == 'I') {
      selector[1] = consume_base(&in_dna_seq);
      if (selector[1] == 'I') {
        selector[2] = consume_base(&in_dna_seq);
      }
    }

    if (strcmp(selector, "C") == 0) {
      emit_pitem(out_pattern_seq, (pitem_t) {.type = PITEM_BASE, .base = 'I'});
      continue;
    }
    if (strcmp(selector, "F") == 0) {
      emit_pitem(out_pattern_seq, (pitem_t) {.type = PITEM_BASE, .base = 'C'});
      continue;
    }
    if (strcmp(selector, "P") == 0) {
      emit_pitem(out_pattern_seq, (pitem_t) {.type = PITEM_BASE, .base = 'F'});
      continue;
    }
    if (strcmp(selector, "IC") == 0) {
      emit_pitem(out_pattern_seq, (pitem_t) {.type = PITEM_BASE, .base = 'P'});
      continue;
    }
    if (strcmp(selector, "IP") == 0) {
      int n = nat(&in_dna_seq);
      if (n == STATUS_FINISH) {
        break;
      }
      emit_pitem(out_pattern_seq, (pitem_t) {.type = PITEM_SKIP_N, .skip = n});
      continue;
    }
    if (strcmp(selector, "IF") == 0) {
      /* Special consumption of extra base (unused) for 'IF' case. */
      consume_base(&in_dna_seq);
      /* Fixed size buffer for now. consts() has overrun detection, so this */
      /* may be optimized, if it causes issues.                             */
      char* consts_buf = (char *) malloc(CONSTS_BUF_SIZE);
      consts(&in_dna_seq, consts_buf, CONSTS_BUF_SIZE);
      emit_pitem(out_pattern_seq, (pitem_t) {.type = PITEM_DNA_SEQ, .dna_seq_ptr = consts_buf});
      continue;
    }
    if (strcmp(selector, "IIP") == 0) {
      lvl++;
      emit_pitem(out_pattern_seq, (pitem_t) {.type = PITEM_OPEN_GROUP});
      continue;
    }
    if (strcmp(selector, "IIC") == 0 || strcmp(selector, "IIF") == 0) {
      if (lvl == 0) {
        break;
      }
      lvl--;
      emit_pitem(out_pattern_seq, (pitem_t) {.type = PITEM_CLOSE_GROUP});
      continue;
    }
    if (strcmp(selector, "III") == 0) {
      for (int i = 0; i < 7; i++) {
        *out_rna++ = consume_base(&in_dna_seq);
      }
      continue;
    }

    /* Did not match any selectors, terminating. */
    break;
  }

  return STATUS_OK;
}
