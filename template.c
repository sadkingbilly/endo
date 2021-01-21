#include "common.h"
#include "template.h"

void emit_titem(titem_seq_t* out_seq, titem_t item) {
  *(out_seq->end) = item;
  out_seq->end++;
  assert((out_seq->end - out_seq->start) < out_seq->size);
}

titem_seq_t* init_template_seq() {
  titem_seq_t* seq = (titem_seq_t*) malloc(sizeof(titem_seq_t));
  seq->start = (titem_t*) malloc(TEMPLATE_SIZE * sizeof(titem_t));
  seq->end = seq->start;
  seq->size = TEMPLATE_SIZE;
  return seq;
}

void free_template_seq(titem_seq_t* seq) {
  free(seq->start);
  free(seq);
}

/* Reads from and advances dna->cur. */
titem_seq_t* template(dna_seq_t* dna, rna_t* rna) {
  titem_seq_t* out = init_template_seq();

  while (1) {
    char selector[4] = {consume_base(dna), '\0', '\0', '\0'};
    if (selector[0] == 'I') {
      selector[1] = consume_base(dna);
      if (selector[1] == 'I') {
        selector[2] = consume_base(dna);
      }
    }

    if (strcmp(selector, "C") == 0) {
      emit_titem(out, (titem_t) {.type = TITEM_BASE, .base = 'I'});
      continue;
    }
    if (strcmp(selector, "F") == 0) {
      emit_titem(out, (titem_t) {.type = TITEM_BASE, .base = 'C'});
      continue;
    }
    if (strcmp(selector, "P") == 0) {
      emit_titem(out, (titem_t) {.type = TITEM_BASE, .base = 'F'});
      continue;
    }
    if (strcmp(selector, "IC") == 0) {
      emit_titem(out, (titem_t) {.type = TITEM_BASE, .base = 'P'});
      continue;
    }
    if (strcmp(selector, "IF") == 0 || strcmp(selector, "IP") == 0) {
      /* Calls to nat() advance dna->cur and may terminate the program by calling finish(). */
      int l = nat(dna, rna);
      int n = nat(dna, rna);
      emit_titem(out, (titem_t) {.type = TITEM_REF, .ref_num = n, .prot_level = l});
      continue;
    }
    if (strcmp(selector, "IIP") == 0) {
      /* Calls to nat() advance dna->cur and may terminate the program by calling finish(). */
      int n = nat(dna, rna);
      emit_titem(out, (titem_t) {.type = TITEM_LEN, .len = n});
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
    if (strcmp(selector, "IIC") == 0 || strcmp(selector, "IIF") == 0) {
      return out;
    }

    /* Did not match any selectors, terminating. */
    break;
  }

  /* finish() does not return. */
  finish(rna);
  /* Not reached. */
  return NULL;
}
