#ifndef REPLACE_H
#define REPLACE_H

#include "common.h"
#include "template.h"
#include "dna_seq.h"

typedef dna_seq_t* env_t[1024];

dna_seq_t* protect(int prot_level, dna_seq_t* dna_seq);
dna_seq_t* asnat(int n);
dna_seq_t* replace(titem_seq_t* template_seq, env_t* env);

#endif  /* REPLACE_H */
