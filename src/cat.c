#include "cat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int args, char** argv) {
  int mask = 0, flag = 0, line_number = 1;
  int last_was_newline = 1, count_newline = 0, start = 0;
  for (int i = 1; i < args && !flag; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "--")) flag = create_mask(argv[i], &mask);
    }
  }
  int files_processed = 0;
  if (flag == 1) files_processed = 1;
  for (int i = 1; i < args && !flag; i++) {
    if (argv[i][0] != '-') {
      FILE* file = fopen(argv[i], "r");
      if (file) {
        process_file(file, mask, &line_number, &last_was_newline,
                     &count_newline, &start);
        fclose(file);
        files_processed++;
      } else {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
        files_processed++;
      }
    }
  }
  if (files_processed == 0) {
    process_file(stdin, mask, &line_number, &last_was_newline, &count_newline,
                 &start);
  }
  return 0;
}

int create_mask(char* argv, int* mask) {
  int flag = 0;
  if (argv[1] == '-') {
    if (!strcmp(argv, "--number-nonblank"))
      (*mask) |= (1 << FLAG_b);
    else if (!strcmp(argv, "--number"))
      (*mask) |= (1 << FLAG_n);
    else if (!strcmp(argv, "--squeeze-blank"))
      (*mask) |= (1 << FLAG_s);
    else {
      fprintf(stderr,
              "cat: unrecognized option \'%s\'\nTry 'cat --help' for more "
              "information.\n",
              argv);
      flag = 1;
    }
  } else {
    for (int j = 1; argv[j] != '\0' && !flag; j++) {
      if (argv[j] == 'b')
        (*mask) |= (1 << FLAG_b);
      else if (argv[j] == 'e') {
        (*mask) |= (1 << FLAG_e);
        (*mask) |= (1 << FLAG_v);
      } else if (argv[j] == 'E')
        (*mask) |= (1 << FLAG_E);
      else if (argv[j] == 'n')
        (*mask) |= (1 << FLAG_n);
      else if (argv[j] == 's')
        (*mask) |= (1 << FLAG_s);
      else if (argv[j] == 't') {
        (*mask) |= (1 << FLAG_t);
        (*mask) |= (1 << FLAG_v);
      } else if (argv[j] == 'T')
        (*mask) |= (1 << FLAG_T);
      else if (argv[j] == 'v')
        (*mask) |= (1 << FLAG_v);
      else {
        fprintf(stderr,
                "cat: invalid option -- '%c'\nTry 'cat --help' for more "
                "information.\n",
                argv[j]);
        flag = 1;
      }
    }
  }
  return flag;
}

void process_file(FILE* file, const int mask, int* line_number,
                  int* last_was_newline, int* count_newline, int* start) {
  int current_char = 0;
  int should_print = 1;
  unsigned char ch = 0;

  while ((current_char = fgetc(file)) != EOF) {
    ch = (unsigned char)current_char;
    should_print = 1;
    if (mask & (1 << FLAG_s)) {
      if (ch != '\n') {
        *count_newline = 0;
        *start = 1;
      } else {
        (*count_newline)++;
        if (((*count_newline) == 2 && !(*start)) || (*count_newline) > 2)
          should_print = 0;
      }
    }
    if ((*last_was_newline) && should_print) {
      if (mask & (1 << FLAG_b)) {
        if (ch != '\n') {
          printf("%6d\t", (*line_number)++);
        }
      } else if (mask & (1 << FLAG_n)) {
        printf("%6d\t", (*line_number)++);
      }
    }
    if (should_print) {
      printf_ch(ch, mask);
      (*last_was_newline) = (ch == '\n');
    }
  }
}

void printf_ch(unsigned char ch, int mask) {
  int special_printed = 0;
  if ((mask & (1 << FLAG_t)) && ch == '\t') {
    printf("^I");
    special_printed = 1;
  } else if ((mask & (1 << FLAG_T)) && ch == '\t') {
    printf("^I");
    special_printed = 1;
  } else if ((mask & (1 << FLAG_e)) && ch == '\n') {
    printf("$\n");
    special_printed = 1;
  } else if ((mask & (1 << FLAG_E)) && ch == '\n') {
    printf("$\n");
    special_printed = 1;
  } else if (mask & (1 << FLAG_v)) {
    spec_printf(ch, &special_printed);
  }
  if (!special_printed) {
    printf("%c", ch);
  }
}

void spec_printf(unsigned char c, int* special_printed) {
  if (c < 32 && c != '\t' && c != '\n') {
    printf("^%c", c + 64);
  } else if (c > 127 && c < 160) {
    printf("M-^%c", c - 64);
  } else if (c >= 160) {
    printf("M-%c", c - 128);
  } else if (c == 127) {
    printf("^?");
  } else {
    printf("%c", c);
  }
  (*special_printed) = 1;
}