#include "common.h"
#include "execute.h"

void execute_test() {
  execute("docs/test.dna", -1);
}

int main(int unused_argc, char** unused_argv) {
  execute_test();
}
