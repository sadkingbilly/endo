#ifndef MATCHREPLACE_H
#define MATCHREPLACE_H

#include "dna_seq.h"
#include "pattern.h"
#include "replace.h"
#include "template.h"

void matchreplace(dna_seq_t* dna, pitem_seq_t* patt, titem_seq_t* tmpl, env_t* env);

#endif  /* MATCHREPLACE_H */
