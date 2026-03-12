#ifndef CAT
#define CAT

#include <stdio.h>
typedef enum {
  FLAG_T,
  FLAG_s,
  FLAG_n,
  FLAG_E,
  FLAG_b,
  FLAG_e,
  FLAG_t,
  FLAG_v
} FLAGS;

int create_mask(char* argv, int* mask);
void spec_printf(unsigned char c, int* special_printed);
void process_file(FILE* file, const int mask, int* line_number,
                  int* last_was_newline, int* count_newline, int* start);
void printf_ch(unsigned char ch, int mask);

#endif