#include "stack.h"
#include "common.h"
#include <stdio.h>

int vm_push(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("PUSH R%d\n", reg);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_push_im32(vm, value);
}

int vm_pop(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("POP R%d\n", reg);

  Number value;
  PROPAGATE_ERROR(vm_pop_im32(vm, &value));

  return vm_set_reg(vm, reg, value);
}
