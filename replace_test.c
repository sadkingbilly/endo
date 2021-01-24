#include "replace.h"
#include "common.h"

void protect_test() {
  dna_seq_t* quoted;
  dna_seq_t* expected;
  dna_seq_t* orig = init_dna_seq_from_str("ICFP");

  quoted = protect(0, orig);
  assert(dna_seq_equal(orig, quoted));
  free_dna_seq(quoted);

  quoted = protect(1, orig);
  expected = init_dna_seq_from_str("CFPIC");
  expected->size = quoted->size;
  assert(dna_seq_equal(expected, quoted));
  free_dna_seq(quoted);
  free_dna_seq(expected);

  quoted = protect(2, orig);
  expected = init_dna_seq_from_str("FPICCF");
  expected->size = quoted->size;
  assert(dna_seq_equal(expected, quoted));
  free_dna_seq(quoted);
  free_dna_seq(expected);

  free_dna_seq(orig);
}

#define PROTECT_FAST_TEST_OUT_BUF_SIZE (1024)
void protect_fast_test() {
  char out_buf[PROTECT_FAST_TEST_OUT_BUF_SIZE];
  dna_seq_t* orig = init_dna_seq_from_str("ICFP");
  size_t chars_out;
  char* expected;

  expected = "ICFP";
  chars_out = protect_fast(0, orig, out_buf, PROTECT_FAST_TEST_OUT_BUF_SIZE);
  assert(chars_out == strlen(expected));
  out_buf[chars_out] = '\0';
  assert(strcmp(out_buf, expected) == 0);

  expected = "CFPIC";
  chars_out = protect_fast(1, orig, out_buf, PROTECT_FAST_TEST_OUT_BUF_SIZE);
  assert(chars_out == strlen(expected));
  out_buf[chars_out] = '\0';
  assert(strcmp(out_buf, expected) == 0);

  expected = "FPICCF";
  chars_out = protect_fast(2, orig, out_buf, PROTECT_FAST_TEST_OUT_BUF_SIZE);
  assert(chars_out == strlen(expected));
  out_buf[chars_out] = '\0';
  assert(strcmp(out_buf, expected) == 0);

  free_dna_seq(orig);

  /* Complex test comparing two different implementations. */
  orig = init_dna_seq_from_str("FPICCFCFPICICFP");
  chars_out = protect_fast(5, orig, out_buf, PROTECT_FAST_TEST_OUT_BUF_SIZE);
  out_buf[chars_out] = '\0';
  dna_seq_t* protect_out = protect(5, orig);
  *protect_out->end = '\0';
  assert(strcmp(out_buf, protect_out->start) == 0);

  free_dna_seq(orig);
  free_dna_seq(protect_out);
}

void asnat_test() {
  dna_seq_t* seq = asnat(0);
  *seq->end = '\0';
  assert(strcmp("P", seq->start) == 0);

  seq = asnat(1);
  *seq->end = '\0';
  assert(strcmp("CP", seq->start) == 0);

  seq = asnat(2);
  *seq->end = '\0';
  assert(strcmp("ICP", seq->start) == 0);

  seq = asnat(8);
  *seq->end = '\0';
  assert(strcmp("IIICP", seq->start) == 0);

  seq = asnat(17);
  *seq->end = '\0';
  assert(strcmp("CIIICP", seq->start) == 0);
}

void replace_test() {
  env_t env;
  env[0] = init_dna_seq_from_str("ICFP");
  env[1] = init_dna_seq_from_str("IC");

  titem_seq_t* templates = init_template_seq();
  /* Will generate 'I'. */
  emit_titem(templates, (titem_t) {.type = TITEM_BASE, .base = 'I'});
  /* Will generate protect(2, env[0]) == 'FPICCF'. */
  emit_titem(templates, (titem_t) {.type = TITEM_REF, .ref_num = 0, .prot_level = 2});
  /* Will generate asnat(len(env[1])) == 'ICP'. */
  emit_titem(templates, (titem_t) {.type = TITEM_LEN, .len = 1});

  dna_seq_t* replace_seq = replace(templates, &env);
  dna_seq_t* expected = init_dna_seq_from_str("IFPICCFICP");
  expected->size = replace_seq->size;
  *replace_seq->end = '\0';
  assert(dna_seq_equal(expected, replace_seq));

  free_dna_seq(expected);
  free_dna_seq(replace_seq);
  free_template_seq(templates);
  free_dna_seq(env[0]);
  free_dna_seq(env[1]);
}

int main(int unused_argc, char** unused_argv) {
  init_asnat_table();

  protect_test();
  protect_fast_test();
  asnat_test();
  replace_test();
}
