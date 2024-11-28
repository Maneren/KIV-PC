#include "conditions.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char cmp_flags(Word a, Word b) {
  return ((a == b) * FLAG_EQ) | ((a < b) * FLAG_LT) | ((a > b) * FLAG_GT);
}

INSTRUCTION(cmp_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Word, im32);
  DEBUG_PRINT_INSTRUCTION_IM("CMP", reg, im32);
  vm->flags = cmp_flags(val, im32);
})

INSTRUCTION(cmp_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("CMP", reg_d, reg_s);
  vm->flags = cmp_flags(val_d, val_s);
})

INSTRUCTION(jmp_im32, {
  READ_ARG(Word, address);
  DEBUG_PRINT("JMP 0x%08X\n", address);
  ASSERT(address >= 0 && (size_t)address < vm->code_size,
         "Relative jump to invalid address 0x%08X", address);
  vm->IP = address;
})

INSTRUCTION(jmp_reg, {
  READ_REG_ARG(reg, address);
  DEBUG_PRINT("JMP R%hhX\n", reg);
  ASSERT(address >= 0 && (size_t)address < vm->code_size,
         "Relative jump to invalid address 0x%08X", address);
  vm->IP = address;
})

#define CONDITIONAL_JUMP(name, cond, debug_name)                               \
  INSTRUCTION(name##_im32, {                                                   \
    READ_ARG(Word, address);                                                   \
    DEBUG_PRINT(#debug_name " 0x%08X\n", address);                             \
    if (cond) {                                                                \
      vm->IP += (size_t)address;                                               \
      ASSERT(vm->IP < vm->code_size,                                           \
             "Jump to address 0x%08lX outside code segment", vm->IP);          \
      vm->flags = 0;                                                           \
    }                                                                          \
  })

CONDITIONAL_JUMP(je, (vm->flags & FLAG_EQ), JE)
CONDITIONAL_JUMP(jne, (!(vm->flags & FLAG_EQ)), JNE)
CONDITIONAL_JUMP(jl, (vm->flags & FLAG_LT), JL)
CONDITIONAL_JUMP(jg, (vm->flags & FLAG_GT), JG)
CONDITIONAL_JUMP(jge, (vm->flags & FLAG_GT || vm->flags & FLAG_EQ), JGE)
CONDITIONAL_JUMP(jle, (vm->flags & FLAG_LT || vm->flags & FLAG_EQ), JLE)
CONDITIONAL_JUMP(jnl, (vm->flags & FLAG_GT || vm->flags & FLAG_EQ), JNL)
CONDITIONAL_JUMP(jng, (vm->flags & FLAG_LT || vm->flags & FLAG_EQ), JNG)
