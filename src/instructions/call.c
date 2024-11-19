#include "common.h"
#include "stack.h"
#include <stdio.h>

INSTRUCTION(call_im32, {
  READ_ARG(Number, address);
  DEBUG_PRINT("CALL 0x%04X\n", address);
  ASSERT(address >= 0 && (size_t)address < vm->code_size,
         "Call to invalid address 0x%08X", address);
  PROPAGATE_ERROR(vm_push_im32(vm, (Number)vm->IP));
  vm->IP = address;
})

INSTRUCTION(call_reg, {
  READ_REG_ARG(reg, address);
  DEBUG_PRINT("CALL R%02hhX\n", reg);
  ASSERT(address >= 0 && (size_t)address < vm->code_size,
         "Call to invalid address 0x%08X", address);
  PROPAGATE_ERROR(vm_push_im32(vm, (Number)vm->IP));
  vm->IP = address;
})

INSTRUCTION(ret, {
  DEBUG_PRINT("RET\n");
  ASSERT(vm->registers.SP > 0, "Return from empty stack");
  Number address = vm->registers.SP;
  ASSERT(address >= 0 && (size_t)address < vm->code_size,
         "Return to invalid address 0x%08X", address);
  vm->IP = address;
})
