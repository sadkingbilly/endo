#include "common.h"
#include "dna_seq.h"
#include "replace.h"

#define PROTECT_TMP_BUF_SIZE (1024)

dna_seq_t* asnat_table[ASNAT_TABLE_SIZE];
int max_protect_level = 0;

void print_max_protect_level() {
  printf("max_protect_level=%d\n", max_protect_level);
}

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
  if (prot_level > max_protect_level) {
    max_protect_level = prot_level;
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

#define PROTECT_SPECIAL_CHARS_SIZE (6)
char* protect_special_chars[PROTECT_SPECIAL_CHARS_SIZE] = {
  "",
  "IC",
  "CF",
  "FP",
  "PIC",
  "ICCF",
};

/* Returns how many bases were actually written into out_buf. */
/* Does not allocate memory.                                  */
/* This actually turned out to be slower than protect(), so   */
/* currently not in use.                                      */
size_t protect_fast(int prot_level, dna_seq_t* dna_seq, char* out_buf, size_t out_buf_size) {
  /* Copy input DNA into out_buf. */
  size_t dna_seq_size = dna_seq->end - dna_seq->start;
  assert(dna_seq_size < out_buf_size);
  memcpy(out_buf, dna_seq->start, dna_seq_size);
  if (prot_level == 0) {
    return dna_seq_size;
  }
  if (prot_level > max_protect_level) {
    max_protect_level = prot_level;
  }

  /* Apply protection requested number of times. As 'P' requires special */
  /* treatment, it is replaced by special chars, to do it in place.      */
  size_t out_size;
  while(prot_level--) {
    out_size = 0;
    for (int i = 0; i < dna_seq_size; i++) {
      if (out_buf[i] == 'I') {
        out_buf[i] = 'C';
        out_size++;
        continue;
      }
      if (out_buf[i] == 'C') {
        out_buf[i] = 'F';
        out_size++;
        continue;
      }
      if (out_buf[i] == 'F') {
        out_buf[i] = 'P';
        out_size++;
        continue;
      }
      if (out_buf[i] == 'P') {
        out_buf[i] = 1;
        out_size += 2;
        continue;
      }
      /* Can only be a special char at this point. */
      out_buf[i] += 1;
      out_size += strlen(protect_special_chars[out_buf[i]]);
    }
  }
  out_buf[dna_seq_size] = '\0';
  /* Expand special chars, filling out_buf in reverse order, */
  /* to avoid another copy. */
  assert(out_size < out_buf_size);
  char* out_ptr = out_buf + out_size - 1;
  char* in_ptr = out_buf + dna_seq_size - 1;
  while (in_ptr >= out_buf) {
    if (*in_ptr == 'I' || *in_ptr == 'C' || *in_ptr == 'F' || *in_ptr == 'P') {
      *out_ptr = *in_ptr;
      out_ptr--;
      in_ptr--;
    } else {
      /* Expansion of special character. */
      assert(*in_ptr < PROTECT_SPECIAL_CHARS_SIZE);
      char* exp_start = protect_special_chars[*in_ptr];
      size_t exp_size = strlen(exp_start);
      /* out_ptr points to the position where the next output char should go.   */
      /* So if we need to write 2 chars, we need to do it a position out_ptr-1, */
      /* and so on. After the write we still need to decrement out_ptr.         */
      out_ptr = out_ptr - exp_size + 1;
      memcpy(out_ptr, exp_start, exp_size);
      out_ptr--;
      in_ptr--;
    }
  }

  return out_size;
}

/* Sequences are cached, caller should not attempt to free them. */
/* Not thread-safe.                                              */
dna_seq_t* asnat(int n) {
  dna_seq_t* out;

#ifdef USE_ASNAT_CACHE
  if (!(n >= 0 && n < ASNAT_TABLE_SIZE)) {
    printf("Argument to asnat() exceeds table size, n=%d\n", n);
    assert(0);
  }
  if (asnat_table[n] != NULL) {
    return asnat_table[n];
  }

  asnat_table[n] = init_dna_seq();
  out = asnat_table[n];
#else
  out = init_dna_seq();
#endif  /* USE_ASNAT_CACHE */

  while (n) {
    if (n % 2) {
      append_to_dna_seq(out, 'C');
    } else {
      append_to_dna_seq(out, 'I');
    }
    n = n / 2;
  }
  append_to_dna_seq(out, 'P');
  return out;
}

#define PROTECT_FAST_OUT_BUF_SIZE (32*1024*1024)
char* protect_fast_out_buf = NULL;

/* replace() implementation below is different from the one described in the */
/* doc: it just generates a new DNA sequence, which caller must then prepend */
/* to the existing DNA.                                                      */
dna_seq_t* replace(titem_seq_t* template_seq, env_t* env) {
  dna_seq_t* asnat_dna_seq;
  dna_seq_t* protect_dna_seq;
  dna_seq_t* env_seq;
  dna_seq_t* out = init_dna_seq();
  titem_t* titem_ptr = template_seq->start;
  if (protect_fast_out_buf == NULL) {
    protect_fast_out_buf = (char*) malloc(PROTECT_FAST_OUT_BUF_SIZE);
  }

  while (titem_ptr != template_seq->end) {
    switch (titem_ptr->type) {
      case TITEM_BASE:
        append_to_dna_seq(out, titem_ptr->base);
        break;
      case TITEM_REF:
        env_seq = (*env)[titem_ptr->ref_num];
        protect_dna_seq = protect(titem_ptr->prot_level, env_seq);
        append_to_dna_seq_from_ptr(out, protect_dna_seq->start,
                                   protect_dna_seq->end - protect_dna_seq->start);
        free_dna_seq(protect_dna_seq);
        break;
      case TITEM_LEN:
        env_seq = (*env)[titem_ptr->len];
        asnat_dna_seq = asnat(env_seq->end - env_seq->start);
        int asnat_dna_seq_size = asnat_dna_seq->end - asnat_dna_seq->start;
        append_to_dna_seq_from_ptr(out, asnat_dna_seq->start, asnat_dna_seq_size);
#ifndef USE_ASNAT_CACHE
	/* When cache is not used, asnat() allocates a new sequence. */
	free_dna_seq(asnat_dna_seq);
#endif  /* USE_ASNAT_CACHE */
        break;
    }
    titem_ptr++;
  }

  return out;
}
