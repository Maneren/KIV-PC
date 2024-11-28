#include "../defs.h"
#include "common.h"
#include <stdio.h>

// Push a value onto the stack
//
// If the stack is full, double its size
int push(VM *vm, Word im32) {
  size_t address = (size_t)vm->registers.SP;
  vm->registers.SP += sizeof(Word);
  size_t required_size = address + sizeof(Word);
  if (required_size >= vm->stack_size) {
    size_t new_size = max(vm->stack_size * 2, required_size);
    SAFE_REALLOCATE(vm->stack_segment, new_size, sizeof(Word));
    vm->stack_size = new_size;
  }
  BYTES_FROM(Word, im32, &vm->stack_segment[address]);
  return EXIT_SUCCESS;
}

// Pop a value from the stack
int pop(VM *vm, Word *out) {
  vm->registers.SP -= sizeof(Word);
  ASSERT(vm->registers.SP >= 0 &&
             (size_t)vm->registers.SP + sizeof(Word) <= vm->stack_size,
         "Read outside stack segment from address 0x%08X", vm->registers.SP);
  BYTES_TO(Word, *out, &vm->stack_segment[vm->registers.SP]);
  return EXIT_SUCCESS;
}

INSTRUCTION(push, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("PUSH R%02hhX\n", reg);
  TRY(push(vm, val));
})

INSTRUCTION(pop, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("POP R%02hhX\n", reg);
  TRY(pop(vm, &val));
  WRITE_REG(reg, val);
})

INSTRUCTION(call_im32, {
  READ_ARG(Word, address);
  DEBUG_PRINT("CALL 0x%04X\n", address);
  ASSERT(address >= 0 && (size_t)address < vm->code_size,
         "Call to invalid address 0x%08X", address);
  TRY(push(vm, vm->IP));
  vm->IP = address;
})

INSTRUCTION(call_reg, {
  READ_REG_ARG(reg, address);
  DEBUG_PRINT("CALL R%02hhX\n", reg);
  ASSERT(address >= 0 && (size_t)address < vm->code_size,
         "Call to invalid address 0x%08X", address);
  TRY(push(vm, vm->IP));
  vm->IP = address;
})

INSTRUCTION(ret, {
  DEBUG_PRINT("RET\n");
  ASSERT(vm->registers.SP > 0, "Return from empty stack");
  Word address;
  TRY(pop(vm, &address));
  ASSERT(address >= 0 && (size_t)address < vm->code_size,
         "Return to invalid address 0x%08X", address);
  vm->IP = address;
})
