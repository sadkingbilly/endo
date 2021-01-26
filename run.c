#include "common.h"
#include "execute.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("ERROR: one argument (max iterations) required.\n");
    return 1;
  }
  char *end;
  long max_iters = strtol(argv[1], &end, 10);
  if (end == argv[1] || end != argv[1] + strlen(argv[1])) {
    printf("ERROR: failed to parse the max iterations argument: %s\n", argv[1]);
    return 1;
  }
  execute(ENDO_DNA_FILENAME, max_iters);
  return 0;
}
