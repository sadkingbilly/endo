#ifndef COMMON_H
#define COMMON_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATUS_OK     (1)
#define STATUS_FAIL   (0)
#define STATUS_FINISH (-1)

char consume_base(char **ptr_ptr);
int nat(char** dna_seq_ptr_ptr);

#endif  /* COMMON_H */
