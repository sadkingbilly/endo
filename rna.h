#ifndef RNA_H
#define RNA_H

#define RNA_SIZE (500000)

typedef struct {
  /* End marker, pointing to the next available slot. */
  char** end;
  /* Array of null-terminated strings holding RNA fragments */
  char* data[RNA_SIZE];
} rna_t;

void init_rna(rna_t* rna);
void emit_rna(rna_t* rna, char* src, size_t size);
void free_rna_data(rna_t* rna);
void finish(rna_t* rna);

#endif  /* RNA_H */
