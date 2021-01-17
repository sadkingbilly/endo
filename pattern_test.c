#include "pattern.h"
#include "common.h"

void pattern_test() {
  pattern_seq_t out_pattern;
  assert(pattern("CIIC", NULL, out_pattern));  
  assert(out_pattern[0].type == BASE);
  assert(out_pattern[0].base == 'I');
  assert(out_pattern[1].type == EMPTY);

  assert(pattern("IIPIPICPIICICIIF", NULL, out_pattern));
  assert(out_pattern[0].type == OPEN_GROUP);
  assert(out_pattern[1].type == SKIP_N);
  assert(out_pattern[1].skip == 2);
  assert(out_pattern[2].type == CLOSE_GROUP);
  assert(out_pattern[3].type == BASE);
  assert(out_pattern[3].base == 'P');
  assert(out_pattern[4].type == EMPTY);
}

int main(int unused_argc, char** unused_argv) {
  pattern_test();
}
