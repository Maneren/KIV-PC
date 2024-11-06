#ifndef INCLUDE_INSTRUCTIONS_COMMON_H_
#define INCLUDE_INSTRUCTIONS_COMMON_H_

#include "../vm.h"
#include <stdlib.h>

// Run the code and return if it returns unsuccessful erorr code
#define PROPAGATE_ERROR(code)                                                  \
  {                                                                            \
    int status = (code);                                                       \
    if (status != EXIT_SUCCESS)                                                \
      return status;                                                           \
  }

// Assert that a condition is true and return if it is not
// Optionally set a custom error message
#define ASSERT(condition, ...)                                                 \
  if (!(condition)) {                                                          \
    snprintf(vm->error_msg, VM_ERROR_BUFFER_SIZE, __VA_ARGS__);                \
    return EXIT_FAILURE;                                                       \
  }

#define DEBUG_PRINT(...)                                                       \
  if (vm->debug)                                                               \
  printf(__VA_ARGS__)

int vm_set_reg(VM *vm, Byte reg, Number im32);
int vm_get_reg(VM *vm, Byte reg, Number *out);

int vm_code_read_reg(VM *vm, Byte *out);
int vm_code_read_im32(VM *vm, Number *out);

int vm_read_im32(VM *vm, Number address, Number *out);
int vm_write_im32(VM *vm, Number address, Number im32);

int vm_push_im32(VM *vm, Number im32);
int vm_pop_im32(VM *vm, Number *out);

int vm_nop(VM *vm);

#endif // INCLUDE_INSTRUCTIONS_COMMON_H_
