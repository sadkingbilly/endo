#include "common.h"
#include "pattern.h"

void emit_pitem(pitem_seq_t* out_seq, pitem_t item) {
  *(out_seq->end) = item;
  out_seq->end++;
  assert((out_seq->end - out_seq->start) < out_seq->size);
}

pitem_seq_t* init_pattern_seq() {
  pitem_seq_t* seq = (pitem_seq_t*) malloc(sizeof(pitem_seq_t));
  seq->start = (pitem_t*) malloc(PATTERN_SIZE * sizeof(pitem_t));
  seq->end = seq->start;
  seq->size = PATTERN_SIZE;
  return seq;
}

void free_pattern_seq(pitem_seq_t* pitem_seq) {
  free(pitem_seq->start);
  free(pitem_seq);
}

/* Reads from and advances dna->cur. */
dna_seq_t* consts(dna_seq_t* dna) {
  dna_seq_t* out = init_dna_seq();
  char* next_ptr;
  while (dna->cur != dna->end) {
    switch (*dna->cur) {
      case 'C':
        append_to_dna_seq(out, 'I');
        dna->cur++;
        break;
      case 'F':
        append_to_dna_seq(out, 'C');
        dna->cur++;
        break;
      case 'P':
        dna->cur++;
        append_to_dna_seq(out, 'F');
        break;
      case 'I':
        next_ptr = dna->cur + 1;
        if (next_ptr != dna->end && *next_ptr == 'C') {
          append_to_dna_seq(out, 'P');
          dna->cur++;
          dna->cur++;
        } else {
          return out;
        }
    }
  }
  return out;
}

/* Reads from and advances dna->cur. Does not alter dna in other ways. */
pitem_seq_t* pattern(dna_seq_t* dna, rna_t* rna) {
  int lvl = 0;
  pitem_seq_t* out = init_pattern_seq();

  while (1) {
    char selector[4] = {consume_base(dna), '\0', '\0', '\0'};
    if (selector[0] == 'I') {
      selector[1] = consume_base(dna);
      if (selector[1] == 'I') {
        selector[2] = consume_base(dna);
      }
    }

    if (strcmp(selector, "C") == 0) {
      emit_pitem(out, (pitem_t) {.type = PITEM_BASE, .base = 'I'});
      continue;
    }
    if (strcmp(selector, "F") == 0) {
      emit_pitem(out, (pitem_t) {.type = PITEM_BASE, .base = 'C'});
      continue;
    }
    if (strcmp(selector, "P") == 0) {
      emit_pitem(out, (pitem_t) {.type = PITEM_BASE, .base = 'F'});
      continue;
    }
    if (strcmp(selector, "IC") == 0) {
      emit_pitem(out, (pitem_t) {.type = PITEM_BASE, .base = 'P'});
      continue;
    }
    if (strcmp(selector, "IP") == 0) {
      /* Calls to nat() advance dna->cur and may terminate the program by calling finish(). */
      int n = nat(dna, rna);
      emit_pitem(out, (pitem_t) {.type = PITEM_SKIP_N, .skip = n});
      continue;
    }
    if (strcmp(selector, "IF") == 0) {
      /* Special consumption of extra base (unused) for 'IF' case. */
      consume_base(dna);
      emit_pitem(out, (pitem_t) {.type = PITEM_DNA_SEQ, .dna_seq = consts(dna)});
      continue;
    }
    if (strcmp(selector, "IIP") == 0) {
      lvl++;
      emit_pitem(out, (pitem_t) {.type = PITEM_OPEN_GROUP});
      continue;
    }
    if (strcmp(selector, "IIC") == 0 || strcmp(selector, "IIF") == 0) {
      if (lvl == 0) {
        return out;
      }
      lvl--;
      emit_pitem(out, (pitem_t) {.type = PITEM_CLOSE_GROUP});
      continue;
    }
    if (strcmp(selector, "III") == 0) {
      size_t rna_size = dna->end - dna->cur;
      if (rna_size > 7) {
        rna_size = 7;
      }
      emit_rna(rna, dna->cur, rna_size);
      dna->cur += rna_size;
      continue;
    }

    /* Did not match any selectors, terminating. */
    break;
  }

  /* finish() does not return. */
  finish(rna);
  /* Not reached. */
  return NULL;
}
