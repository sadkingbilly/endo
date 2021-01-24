#ifndef REPLACE_H
#define REPLACE_H

#include "common.h"
#include "template.h"
#include "dna_seq.h"

#define ENV_SIZE (1024)
#define ASNAT_TABLE_SIZE (1024 * 1024 * 16)

typedef dna_seq_t* env_t[ENV_SIZE];

dna_seq_t* protect(int prot_level, dna_seq_t* dna_seq);
size_t protect_fast(int prot_level, dna_seq_t* dna_seq, char* out_buf, size_t out_buf_size);
void print_max_protect_level();
void init_asnat_table();
dna_seq_t* asnat(int n);
dna_seq_t* replace(titem_seq_t* template_seq, env_t* env);

#endif  /* REPLACE_H */
