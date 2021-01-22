#ifndef REPLACE_H
#define REPLACE_H

#include "common.h"
#include "template.h"
#include "dna_seq.h"

#define ENV_SIZE (1024)

typedef dna_seq_t* env_t[ENV_SIZE];

dna_seq_t* protect(int prot_level, dna_seq_t* dna_seq);
dna_seq_t* asnat(int n);
dna_seq_t* replace(titem_seq_t* template_seq, env_t* env);

#endif  /* REPLACE_H */
