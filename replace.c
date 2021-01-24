#include "common.h"
#include "dna_seq.h"
#include "replace.h"

#define PROTECT_TMP_BUF_SIZE (1024)

dna_seq_t* asnat_table[ASNAT_TABLE_SIZE];

void init_asnat_table() {
  for (int i = 0; i < ASNAT_TABLE_SIZE; i++) {
    asnat_table[i] = NULL;
  }
}

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

/* Sequences are cached, caller should not attempt to free them. */
/* Not thread-safe.                                              */
dna_seq_t* asnat(int n) {
  if (!(n >= 0 && n < ASNAT_TABLE_SIZE)) {
    printf("failure n=%d\n", n);
    assert(0);
  }
  if (asnat_table[n] != NULL) {
    return asnat_table[n];
  }

  int n_orig = n;
  asnat_table[n_orig] = init_dna_seq();
  while (n) {
    if (n % 2) {
      append_to_dna_seq(asnat_table[n_orig], 'C');
    } else {
      append_to_dna_seq(asnat_table[n_orig], 'I');
    }
    n = n / 2;
  }
  append_to_dna_seq(asnat_table[n_orig], 'P');
  return asnat_table[n_orig];
}

/* replace() implementation below is different from the one described in the */
/* doc: it just generates a new DNA sequence, which caller must then prepend */
/* to the existing DNA.                                                      */
dna_seq_t* replace(titem_seq_t* template_seq, env_t* env) {
  dna_seq_t* asnat_dna_seq;
  dna_seq_t* protect_dna_seq;
  dna_seq_t* env_seq;
  dna_seq_t* out = init_dna_seq();
  titem_t* titem_ptr = template_seq->start;

  while (titem_ptr != template_seq->end) {
    switch (titem_ptr->type) {
      case TITEM_BASE:
        append_to_dna_seq(out, titem_ptr->base);
        break;
      case TITEM_REF:
        env_seq = (*env)[titem_ptr->ref_num];
        protect_dna_seq = protect(titem_ptr->prot_level, env_seq);
        int protect_dna_seq_size = protect_dna_seq->end - protect_dna_seq->start;
        append_to_dna_seq_from_ptr(out, protect_dna_seq->start, protect_dna_seq_size);
        free_dna_seq(protect_dna_seq);
        break;
      case TITEM_LEN:
        env_seq = (*env)[titem_ptr->len];
        /* No need to free the sequence returned by asnat(). */
        asnat_dna_seq = asnat(env_seq->end - env_seq->start);
        int asnat_dna_seq_size = asnat_dna_seq->end - asnat_dna_seq->start;
        append_to_dna_seq_from_ptr(out, asnat_dna_seq->start, asnat_dna_seq_size);
        break;
    }
    titem_ptr++;
  }

  return out;
}
