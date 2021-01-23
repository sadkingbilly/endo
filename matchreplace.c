#include "common.h"
#include "matchreplace.h"

#define C_SIZE (1024)

void free_env(env_t* env_ptr, size_t count) {
  for (int i = 0; i < count; i++) {
    free_dna_seq((*env_ptr)[i]);
  }
}

void print_dna_seq(char* ptr, size_t size) {
  fwrite(ptr, 1, size, stdout);
  printf("\n");
}

dna_seq_t* matchreplace(dna_seq_t* in_dna, pitem_seq_t* patt, titem_seq_t* tmpl) {
  size_t i = 0;
  dna_seq_t* match_dna;
  int dna_size, match_size, match_pos, min_n, max_n, matched;
  /* Since c is used as stack, elements are stored in it in reverse order, */
  /* compared to the specification.                                        */
  int c[C_SIZE];
  int* c_end = c;

  /* Elements added to the environment need to be freed on return. */
  env_t env;
  size_t env_count = 0;

  printf("[matchreplace#0] pattern_size=%d.\n", patt->end - patt->start);
  printf("[matchreplace#1] in_dna_cur=%d in_dna_end=%d.\n", in_dna->cur - in_dna->start, in_dna->end - in_dna->start);

  /* We should not alter dna->cur within this loop, as it defines the relevant    */
  /* input sequence, and that input sequence is not altered within the loop. If   */
  /* we have to return early from the loop, we return the original DNA, "rebased" */
  /* at in_dna->cur.                                                              */
  dna_size = in_dna->end - in_dna->cur;
  dna_seq_t* out_dna = init_dna_seq_from_ptr(in_dna->cur, dna_size);
  for (pitem_t* pitem = patt->start; pitem != patt->end; pitem++) {
    switch(pitem->type) {
      case PITEM_BASE:
        if (*(in_dna->cur + i) == pitem->base) {
          i++;
        } else {
          free_env(&env, env_count);
          return out_dna;
        }
        break;
      case PITEM_SKIP_N:
        i = i + pitem->skip;
        if (i > (in_dna->end - in_dna->cur)) {
          free_env(&env, env_count);
          return out_dna;
        }
        break;
      case PITEM_DNA_SEQ:
        match_dna = pitem->dna_seq;
        match_size = match_dna->end - match_dna->start;
        printf("[matchreplace#dna_seq] match_size=%d.\n", match_size);
        printf("[matchreplace#dna_seq] match_dna=");
        print_dna_seq(match_dna->start, match_size);
        /* We need to verify whether match_dna is a postfix of dna[i..n], */
        /* а sequence of length (n - i), for all possible n >= i.         */
        min_n = i + match_size;
        max_n = dna_size;
        if (max_n < min_n) {
          /* No viable n values due to size constraints. */
          free_env(&env, env_count);
          return out_dna;
        }
        matched = 0;
        printf("[matchreplace#dna_seq] min_n=%d max_n=%d.\n", min_n, max_n);
        for (int n = min_n; n <= max_n; n++) {
          /* match_pos is the position relative to dna->cur where the */
          /* match_dna would start, if it is included in dna[i..n] as */
          /* postfix.                                                 */
          match_pos = n - match_size;
          if (dna_seq_match(in_dna->cur + match_pos, match_dna->start, match_size)) {
            printf("[matchreplace#dna_seq] matched at match_pos=%d.\n", match_pos);
            i = n;
            matched = 1;
            break;
          }
        }
        if (!matched) {
          free_env(&env, env_count);
          return out_dna;
        }
        break;
      case PITEM_OPEN_GROUP:
        /* Push the value to c stack. */
        *c_end = i;
        c_end++;
        assert(c_end - c < C_SIZE);
        break;
      case PITEM_CLOSE_GROUP:
        env[env_count] = init_dna_seq();
        /* We can assume that i is within bounds here, as we check for overruns */
        /* in the other branches.                                               */
        for (int k = *c_end; k < i; k++) {
          append_to_dna_seq(env[env_count], *(in_dna->cur + k));
        }
        /* Advance env_count to the next empty slot. */
        env_count++;
        assert(env_count < ENV_SIZE);
        /* Pop the value from c stack. */  
        c_end--;
        assert(c_end >= c);
        break;
    }
  }
  /* Construct a new DNA by concatenating the sequence returned by replace() */
  /* and the remaining "tail" of current DNA from i to the end.              */
  free_dna_seq(out_dna);
  printf("[matchreplace] no early return, i=%d\n", i);
  assert(in_dna->cur + i <= in_dna->end);
  in_dna->cur += i;
  printf("[matchreplace#2] in_dna_cur=%d in_dna_end=%d.\n", in_dna->cur - in_dna->start, in_dna->end - in_dna->start);
  out_dna = replace(tmpl, &env);
  printf("[matchreplace#3] in_dna_cur=%d in_dna_end=%d.\n", in_dna->cur - in_dna->start, in_dna->end - in_dna->start);
  while (in_dna->cur != in_dna->end) {
    append_to_dna_seq(out_dna, *in_dna->cur);
    in_dna->cur++;
  }
  printf("[matchreplace#4] in_dna_cur=%d in_dna_end=%d.\n", in_dna->cur - in_dna->start, in_dna->end - in_dna->start);
  free_env(&env, env_count);
  return out_dna;
}
