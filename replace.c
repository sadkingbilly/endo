#include "common.h"
#include "dna_seq.h"
#include "replace.h"

#define PROTECT_TMP_BUF_SIZE (1024)

/* The effect of protect() is to quote input sequence prot_level times. */
dna_seq_t* protect(int prot_level, dna_seq_t* dna_seq) {
  dna_seq_t* out = clone_dna_seq(dna_seq);
  if (prot_level == 0) {
    return out;
  }

  dna_seq_t* tmp = init_dna_seq_with_size(2 * dna_seq->size);
  while(prot_level--) {
    for (char* ptr = out->start; ptr != out->end; ptr++) {
      switch (*ptr) {
        case 'I':
          append_to_dna_seq(tmp, 'C');
          break;
        case 'C':
          append_to_dna_seq(tmp, 'F');
          break;
        case 'F':
          append_to_dna_seq(tmp, 'P');
          break;
        case 'P':
          append_to_dna_seq(tmp, 'I');
          append_to_dna_seq(tmp, 'C');
          break;
      }
    }
    /* Potential optimization: use stack variables to minimize allocations/freeing. */
    free_dna_seq(out);
    out = clone_dna_seq(tmp);
    tmp->end = tmp->start;
  }
  free_dna_seq(tmp);

  return out;
}

/* Potential optimization: may be precalculated. */
dna_seq_t* asnat(int n) {
  assert(n >= 0);
  dna_seq_t* dna_seq = init_dna_seq();
  while (n) {
    if (n % 2) {
      append_to_dna_seq(dna_seq, 'C');
    } else {
      append_to_dna_seq(dna_seq, 'I');
    }
    n = n / 2;
  }
  append_to_dna_seq(dna_seq, 'P');
  return dna_seq;
}

void replace(titem_seq_t* template_seq, env_t* env, dna_seq_t* out_dna_seq) {
  titem_t* titem_ptr = template_seq->start;
  dna_seq_t* asnat_dna_seq;
  dna_seq_t* protect_dna_seq;

  while (titem_ptr - template_seq->start < template_seq->size) {
    switch (titem_ptr->type) {
      case TITEM_BASE:
        append_to_dna_seq(out_dna_seq, titem_ptr->base);
        break;
      case TITEM_REF:
        protect_dna_seq = protect(titem_ptr->prot_level, (*env)[titem_ptr->ref_num]);
        for (char* ptr = protect_dna_seq->start; ptr != protect_dna_seq->end; ptr++) {
          append_to_dna_seq(out_dna_seq, *ptr);
        }
        free_dna_seq(protect_dna_seq);
        break;
      case TITEM_LEN:
        asnat_dna_seq = asnat(titem_ptr->len);
        for (char* ptr = asnat_dna_seq->start; ptr != asnat_dna_seq->end; ptr++) {
          append_to_dna_seq(out_dna_seq, *ptr);
        }
        free_dna_seq(asnat_dna_seq);
        break;
    }
    titem_ptr++;
  }
}
