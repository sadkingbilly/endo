#ifndef MATCHREPLACE_H
#define MATCHREPLACE_H

#include "dna_seq.h"
#include "pattern.h"
#include "replace.h"
#include "template.h"

dna_seq_t* matchreplace(dna_seq_t* in_dna, pitem_seq_t* patt, titem_seq_t* tmpl);

#endif  /* MATCHREPLACE_H */
