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

#define INSTRUCTION(name, code)                                                \
  int vm_##name(VM *vm) {                                                      \
    {                                                                          \
      code                                                                     \
    }                                                                          \
    return EXIT_SUCCESS;                                                       \
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

#define DEBUG_PRINT_INSTRUCTION(name, reg1, reg2)                              \
  DEBUG_PRINT(name " R%d, R%d\n", reg1, reg2);

#define DEBUG_PRINT_INSTRUCTION_IM(name, reg, im)                              \
  DEBUG_PRINT(name " R%d, %d\n", reg, im);

#define READ(type, var, address)                                               \
  type var;                                                                    \
  {                                                                            \
    ASSERT(address >= 0 && (size_t)address + sizeof(type) <= vm->data_size,    \
           "Attempted to read " #type                                          \
           " outside of data segment (address 0x%08X) at IP 0x%08lX",          \
           address, vm->IP);                                                   \
    var = *(type *)(vm->data_segment + address);                               \
  }

#define WRITE(type, var, address)                                              \
  ASSERT(address >= 0 && (size_t)address + sizeof(type) <= vm->data_size,      \
         "Attempted to write " #type                                           \
         " outside of data segment (address 0x%08X) at IP 0x%08lX",            \
         address, vm->IP);                                                     \
  *(type *)(vm->data_segment + address) = var;

#define READ_ARG(type, var)                                                    \
  type var;                                                                    \
  {                                                                            \
    ASSERT(vm->IP + sizeof(type) <= vm->code_size,                             \
           "Attempted to read " #type                                          \
           " argument outside of code segment (IP 0x%08lX)",                   \
           vm->IP);                                                            \
    var = *(type *)(vm->code_segment + vm->IP);                                \
    vm->IP += sizeof(type);                                                    \
  }

int vm_get_reg(VM *vm, Byte reg, Number *out);
#define READ_REG(reg, var)                                                     \
  Number var;                                                                  \
  {                                                                            \
    PROPAGATE_ERROR(vm_get_reg(vm, reg, &var));                                \
  }

#define READ_REG_ARG(var, val)                                                 \
  READ_ARG(Byte, var);                                                         \
  READ_REG(var, val);

#define READ_REG_DS_PAIR(reg, val)                                             \
  READ_REG_ARG(reg##_d, val##_d);                                              \
  READ_REG_ARG(reg##_s, val##_s);

int vm_set_reg(VM *vm, Byte reg, Number im32);
#define WRITE_REG(reg, value)                                                  \
  {                                                                            \
    PROPAGATE_ERROR(vm_set_reg(vm, reg, value));                               \
  }

#define BINARY_OPERATION_REG_REG(name, op)                                     \
  INSTRUCTION(name##_reg_reg, {                                                \
    READ_REG_ARG(A, a);                                                        \
    READ_REG_ARG(B, b);                                                        \
    WRITE_REG(A, a op b);                                                      \
  })

#define BINARY_OPERATION_REG_IM32(name, op)                                    \
  INSTRUCTION(name##_reg_im32, {                                               \
    READ_REG_ARG(A, a);                                                        \
    READ_ARG(Number, B);                                                       \
    WRITE_REG(A, a op B);                                                      \
  })

int vm_nop(VM *vm);

#endif // INCLUDE_INSTRUCTIONS_COMMON_H_
