#include "common.h"
#include "matchreplace.h"

void matchreplace(dna_seq_t* dna, pitem_seq_t* patt, titem_seq_t* tmpl, env_t* env) {
  size_t i = 0;
  int c[1024];
  int* c_end = c;
  
  for (pitem_t* pitem = patt->start; pitem != patt->end; pitem++) {
    switch(pitem->type) {
      case PITEM_BASE:
        if (get_from_dna_seq(dna, i) != pitem->base) {
          return;
        }
        i++;
        break;
      case PITEM_SKIP_N:
        i += pitem->skip;
        if (i > (dna->end - dna->start))  {
          return;
        }
        break;
      case PITEM_DNA_SEQ:
        /* TODO: implement. */
        break;
      case PITEM_OPEN_GROUP:
        /* TODO: implement. */
        break;
      case PITEM_CLOSE_GROUP:
        /* TODO: implement. */
        break;
    }
  }
}
