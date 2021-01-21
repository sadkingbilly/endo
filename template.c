#include "common.h"
#include "template.h"

void emit_titem(titem_seq_t* out_seq, titem_t item) {
  assert(out_seq->size);
  assert((out_seq->next - out_seq->start) < out_seq->size);
  *(out_seq->next) = item;
  out_seq->next++;
}

titem_seq_t* init_template_seq(size_t size) {
  titem_seq_t* seq = (titem_seq_t*) malloc(sizeof(titem_seq_t));
  seq->start = (titem_t*) malloc(size * sizeof(titem_t));
  seq->next = seq->start;
  seq->size = size;
  return seq;
}

/* Reads from and advances dna->cur. */
int template(dna_seq_t* dna, rna_t* rna, titem_seq_t* out_template_seq) {
  while (1) {
    char selector[4] = {consume_base(dna), '\0', '\0', '\0'};
    if (selector[0] == 'I') {
      selector[1] = consume_base(dna);
      if (selector[1] == 'I') {
        selector[2] = consume_base(dna);
      }
    }

    if (strcmp(selector, "C") == 0) {
      emit_titem(out_template_seq, (titem_t) {.type = TITEM_BASE, .base = 'I'});
      continue;
    }
    if (strcmp(selector, "F") == 0) {
      emit_titem(out_template_seq, (titem_t) {.type = TITEM_BASE, .base = 'C'});
      continue;
    }
    if (strcmp(selector, "P") == 0) {
      emit_titem(out_template_seq, (titem_t) {.type = TITEM_BASE, .base = 'F'});
      continue;
    }
    if (strcmp(selector, "IC") == 0) {
      emit_titem(out_template_seq, (titem_t) {.type = TITEM_BASE, .base = 'P'});
      continue;
    }
    if (strcmp(selector, "IF") == 0 || strcmp(selector, "IP") == 0) {
      /* Calls to nat() advance dna->cur and may terminate the program by calling finish(). */
      int l = nat(dna, rna);
      int n = nat(dna, rna);
      emit_titem(out_template_seq, (titem_t) {.type = TITEM_REF, .ref_num = n, .prot_level = l});
      continue;
    }
    if (strcmp(selector, "IIP") == 0) {
      /* Calls to nat() advance dna->cur and may terminate the program by calling finish(). */
      int n = nat(dna, rna);
      emit_titem(out_template_seq, (titem_t) {.type = TITEM_LEN, .len = n});
      continue;
    }
    if (strcmp(selector, "III") == 0) {
      for (int i = 0; i < 7; i++) {
        *rna++ = consume_base(dna);
      }
      continue;
    }

    /* Did not match any selectors, terminating. */
    break;
  }

  return STATUS_OK;
}
