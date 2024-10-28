#include "stack.h"
#include "common.h"
#include <stdio.h>

int vm_push(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("PUSH R%d\n", reg);

  vm->registers.SP += sizeof(Number);

  ASSERT(vm->registers.SP >= 0 && (size_t)vm->registers.SP < vm->stack_size);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  *(Number *)(vm->stack_segment + vm->registers.SP) = value;

  return 0;
}

int vm_pop(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("POP R%d\n", reg);

  ASSERT(vm->registers.SP >= 0 && (size_t)vm->registers.SP < vm->stack_size);

  Number value = *(Number *)(vm->stack_segment + vm->registers.SP);

  PROPAGATE_ERROR(vm_set_reg(vm, reg, value));

  vm->registers.SP -= sizeof(Number);

  return 0;
}
