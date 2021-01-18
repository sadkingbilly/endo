#include "common.h"
#include "replace.h"

char* protect(env_t* env, int ref_num, int prot_level) {
  return NULL;
}

char* asnat(env_t* env, int len) {
  return NULL;
}

void replace(titem_seq_t* template_seq, env_t* env, char** out_dna_seq) {
  assert(template_seq->size > 0);
  titem_t* titem_ptr = template_seq->start;
  char* protect_ptr;
  char* asnat_ptr;
  while (titem_ptr - template_seq->start < template_seq->size) {
    switch (titem_ptr->type) {
      case TITEM_BASE:
        **out_dna_seq = titem_ptr->base;
        (*out_dna_seq)++;
        break;
      case TITEM_REF:
        protect_ptr = protect(env, titem_ptr->ref_num, titem_ptr->prot_level);
        while (*protect_ptr != '\0') {
          **out_dna_seq = *protect_ptr;
          (*out_dna_seq)++;
          protect_ptr++;
        }
        break;
      case TITEM_LEN:
        asnat_ptr = asnat(env, titem_ptr->len);
        while (*asnat_ptr != '\0') {
          **out_dna_seq = *protect_ptr;
          (*out_dna_seq)++;
          asnat_ptr++;
        }
        break;
    }
    titem_ptr++;
  }
}
