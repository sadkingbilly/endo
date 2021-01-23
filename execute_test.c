#include "common.h"
#include "execute.h"

void execute_test() {
  execute("docs/test.dna");
}

int main(int unused_argc, char** unused_argv) {
  execute_test();
}
