#include "common.h"
#include "matchreplace.h"

#define C_SIZE (1024)

void matchreplace(dna_seq_t* dna, pitem_seq_t* patt, titem_seq_t* tmpl, env_t* out_env) {
  size_t i = 0;
  dna_seq_t* match_dna;
  dna_seq_t** env_start = (dna_seq_t**) out_env;
  dna_seq_t** env_end = env_start;
  int dna_size, match_size, match_pos, min_n, max_n, matched;
  /* Since c is used as stack, elements are stored in it in reverse order, */
  /* compared to the specification.                                        */
  int c[C_SIZE];
  int* c_end = c;

  /* We should not alter dna->cur within this loop, as it defines the relevant */
  /* input sequence, and that input sequence is not altered within the loop.   */
  for (pitem_t* pitem = patt->start; pitem != patt->end; pitem++) {
    switch(pitem->type) {
      case PITEM_BASE:
        if (*(dna->cur + i) == pitem->base) {
          i++;
        } else {
          return;
        }
        break;
      case PITEM_SKIP_N:
        i = i + pitem->skip;
        if (i > (dna->end - dna->cur)) {
          return;
        }
        break;
      case PITEM_DNA_SEQ:
        match_dna = pitem->dna_seq;
        match_size = match_dna->end - match_dna->start;
        dna_size = dna->end - dna->cur;
        /* We need to verify whether match_dna is a postfix of dna[i..n], */
        /* а sequence of length (n - i), for all possible n >= i.         */
        min_n = i + match_size;
        max_n = dna_size;
        if (max_n < min_n) {
          /* No viable n values due to size constraints. */
          return;
        }
        matched = 0;
        for (int n = min_n; n <= max_n; n++) {
          /* match_pos is the position relative to dna->cur where the */
          /* match_dna would start, if it is included in dna[i..n] as */
          /* postfix.                                                 */
          match_pos = n - match_size;
          if (dna_seq_match(dna->cur + match_pos, match_dna->start, match_size)) {
            i = n;
            matched = 1;
            break;
          }
        }
        if (!matched) {
          return;
        }
        break;
      case PITEM_OPEN_GROUP:
        /* Push the value to c stack. */
        *c_end = i;
        c_end++;
        assert(c_end - c < C_SIZE);
        break;
      case PITEM_CLOSE_GROUP:
        *env_end = init_dna_seq();
        /* We can assume that i is within bounds here, as we check for overruns */
        /* in the other branches.                                               */
        for (int k = *c_end; k < i; k++) {
          append_to_dna_seq(*env_end, *(dna->cur + k));
        }
        /* Advance env_end pointer to the next empty slot. */
        env_end++;
        assert(env_end - env_start < ENV_SIZE);
        /* Pop the value from c stack. */  
        c_end--;
        assert(c_end >= c);
        break;
    }
  }
}
