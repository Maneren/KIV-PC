#ifndef INCLUDE_SEMESTRALKA_VM_H_
#define INCLUDE_SEMESTRALKA_VM_H_

#include <stddef.h>
#include <stdio.h>

#define MEMORY_SIZE 256 * 1024
#define STACK_SIZE 16 * 1024

#define VM_ERROR_BUFFER_SIZE 256

typedef signed int Number;
typedef unsigned char Byte;

enum {
  FLAG_EQ = 1 << 0,
  FLAG_GT = 1 << 1,
  FLAG_LT = 1 << 2,
};

typedef struct {
  Number A;
  Number B;
  Number C;
  Number D;
  Number S;
  Number SP;
} Registers;

typedef struct {
  Registers registers;
  size_t IP;
  Byte *code_segment;
  size_t code_size;
  Byte *data_segment;
  size_t data_size;
  Byte *stack_segment;
  size_t stack_size;
  char *error_msg;
  FILE *output;
  unsigned char flags;
  size_t instructions_count;
  unsigned char debug;
} VM;

int init_vm_from_file(const char *input_filepath, const char *output_filepath,
                      VM *vm);

int vm_run(VM *vm);

void vm_print(const VM *vm);

void vm_free(VM *vm);

#endif // INCLUDE_SEMESTRALKA_VM_H_
