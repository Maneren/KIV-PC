#ifndef INCLUDE_KMEMU_VM_H_
#define INCLUDE_KMEMU_VM_H_

#include <stddef.h>
#include <stdio.h>

#define VM_MEMORY_SIZE 256 * 1024
#define VM_STACK_SIZE 16 * 1024

#define VM_ERROR_BUFFER_SIZE 256

typedef signed int Word;
typedef unsigned char Byte;

enum {
  FLAG_EQ = 1 << 0,
  FLAG_GT = 1 << 1,
  FLAG_LT = 1 << 2,
};

typedef struct {
  Word A;
  Word B;
  Word C;
  Word D;
  Word S;
  Word SP;
} Registers;

typedef enum {
  DEBUG_NONE = 0,
  DEBUG_INSTRUCTION = 1,
  DEBUG_MEMORY = 2,
} DebugLevel;

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
  DebugLevel debug;
} VM;

// Initialize the VM
//
// output_filepath may be NULL, in which case stdout will be used
int init_vm_from_file(const char *input_filepath, const char *output_filepath,
                      VM *vm);

// Run the VM
//
// Assumes that the VM is initialized
int vm_run(VM *vm);

// Print the current state of the VM
void vm_print(const VM *vm);

// Free the memory used by the VM
void vm_free(VM *vm);

#endif
