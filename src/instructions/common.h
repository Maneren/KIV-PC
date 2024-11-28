/**
 * @file common.h
 * @brief Define common macros and helper function for the instruction set
 *
 * Defines macros for
 *  - working with the VM's memory and registers
 *  - error handling
 *  - debug printing
 */

#ifndef INCLUDE_INSTRUCTIONS_COMMON_H_
#define INCLUDE_INSTRUCTIONS_COMMON_H_

#include "../vm.h"
#include <stdlib.h>

// Run the code and propagate return code if not successful
#define TRY(code)                                                              \
  {                                                                            \
    int status = (code);                                                       \
    if (status != EXIT_SUCCESS)                                                \
      return status;                                                           \
  }

// Define an instruction with a name and handler code
#define INSTRUCTION(name, code)                                                \
  int vm_##name(VM *vm) {                                                      \
    {                                                                          \
      code                                                                     \
    }                                                                          \
    return EXIT_SUCCESS;                                                       \
  }

// Set the error message and return the error code
#define THROW_ERROR(code, ...)                                                 \
  {                                                                            \
    snprintf(vm->error_msg, VM_ERROR_BUFFER_SIZE, __VA_ARGS__);                \
    return code;                                                               \
  }

// Assert that a condition is true
//
// On success return EXIT_SUCCESS, else return EXIT_FAILURE and sprintf error
// message
#define ASSERT(condition, ...)                                                 \
  if (!(condition)) {                                                          \
    THROW_ERROR(EXIT_FAILURE, __VA_ARGS__);                                    \
  }

// Print debug information if debug mode is enabled
#define DEBUG_PRINT(...)                                                       \
  if (vm->debug > DEBUG_NONE)                                                  \
  printf(__VA_ARGS__)

// Debug print instruction with two register arguments
#define DEBUG_PRINT_INSTRUCTION(name, reg1, reg2)                              \
  DEBUG_PRINT(name " R%hhX, R%hhX\n", reg1, reg2);

// Debug print instruction with register and immediate arguments
#define DEBUG_PRINT_INSTRUCTION_IM(name, reg, im)                              \
  DEBUG_PRINT(name " R%hhX, 0x%08X\n", reg, im);

// Convert little-endian bytes to a type in platform endianness
#define BYTES_TO(type, var, ptr)                                               \
  var = 0;                                                                     \
  {                                                                            \
    for (size_t i = 0; i < sizeof(type); i++) {                                \
      var |= ((type)(ptr)[i] << (i * 8));                                      \
    }                                                                          \
  }

// Convert a type to bytes in little-endian order
#define BYTES_FROM(type, var, ptr)                                             \
  {                                                                            \
    for (size_t i = 0; i < sizeof(type); i++) {                                \
      *(ptr + i) = (Byte)((var >> (i * 8)) & 0xFF);                            \
    }                                                                          \
  }

// Try to read a value of a given type from the data segment to a given variable
//
// Errors:
//  - VM: Attempted to read outside of the data segment
#define READ(type, var, address)                                               \
  type var;                                                                    \
  {                                                                            \
    ASSERT(address >= 0 && (size_t)address + sizeof(type) <= vm->data_size,    \
           "Attempted to read " #type                                          \
           " outside of the data segment (address 0x%08X) at IP 0x%08lX",      \
           address, vm->IP);                                                   \
    BYTES_TO(type, var, vm->data_segment + address);                           \
  }

// Try to write a value of a given type from a given variable to the data
// segment
//
// Errors:
//  - VM: Attempted to write outside of the data segment
#define WRITE(type, var, address)                                              \
  ASSERT(address >= 0 && (size_t)address + sizeof(type) <= vm->data_size,      \
         "Attempted to write " #type                                           \
         " outside of the data segment (address 0x%08X) at IP 0x%08lX",        \
         address, vm->IP);                                                     \
  BYTES_FROM(type, var, vm->data_segment + address);

// Try to read a value of a given type from the code segment to a given variable
//
// The instruction pointer is advanced by the size of the type
//
// Errors:
//  - VM: Attempted to read outside of the code segment
#define READ_ARG(type, var)                                                    \
  type var;                                                                    \
  {                                                                            \
    ASSERT(vm->IP + sizeof(type) <= vm->code_size,                             \
           "Attempted to read " #type                                          \
           " argument outside of the code segment (IP 0x%08lX)",               \
           vm->IP);                                                            \
    BYTES_TO(type, var, &vm->code_segment[vm->IP]);                            \
    vm->IP += sizeof(type);                                                    \
  }

// Read a value from the given register
int vm_get_reg(VM *vm, Byte reg, Word *out);

// Read a value from the given register
//
// Errors:
//  - VM: Register not found
#define READ_REG(reg, var)                                                     \
  Word var;                                                                    \
  {                                                                            \
    TRY(vm_get_reg(vm, reg, &var));                                            \
  }

// Try to read a value from a register given by an argument
//
// Errors:
//  - VM: Attempted to read outside of the code segment
//  - VM: Register not found
#define READ_REG_ARG(var, val)                                                 \
  READ_ARG(Byte, var);                                                         \
  READ_REG(var, val);

// Try to read a pair of values from registers given by arguments
//
// Assumes that the register arguments are in order `d`estination, `s`ource
//
// Errors:
//  - VM: Attempted to read outside of the code segment
//  - VM: Register not found
#define READ_REG_DS_PAIR(reg, val)                                             \
  READ_REG_ARG(reg##_d, val##_d);                                              \
  READ_REG_ARG(reg##_s, val##_s);

// Set the value of a register
//
// Errors:
//  - VM: Register not found
//  - VM: Attempted to write outside of the data segment
int vm_set_reg(VM *vm, Byte reg, Word im32);

// Try to write a value to a register
//
// Errors:
//  - VM: Register not found
//  - VM: Attempted to write outside of the data segment
#define WRITE_REG(reg, value)                                                  \
  {                                                                            \
    TRY(vm_set_reg(vm, reg, value));                                           \
  }

// Define a binary operation instruction for two registers with a name and
// operator
#define BINARY_OPERATION_REG_REG(name, op)                                     \
  INSTRUCTION(name##_reg_reg, {                                                \
    READ_REG_ARG(A, a);                                                        \
    READ_REG_ARG(B, b);                                                        \
    WRITE_REG(A, a op b);                                                      \
  })

// Define a binary operation instruction for a register and an immediate with a
// name and operator
#define BINARY_OPERATION_REG_IM32(name, op)                                    \
  INSTRUCTION(name##_reg_im32, {                                               \
    READ_REG_ARG(A, a);                                                        \
    READ_ARG(Word, B);                                                         \
    WRITE_REG(A, a op B);                                                      \
  })

// NOP instruction
int vm_nop(VM *vm);

#endif // INCLUDE_INSTRUCTIONS_COMMON_H_
