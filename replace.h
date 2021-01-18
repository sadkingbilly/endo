#ifndef REPLACE_H
#define REPLACE_H

#include "common.h"
#include "template.h"

typedef void env_t;

void replace(titem_seq_t* template_seq, env_t* env, char** out_dna_seq);

#endif  /* REPLACE_H */
