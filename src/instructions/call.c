#include "common.h"
#include "stack.h"
#include <stdio.h>

int vm_call_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("CALL %08X\n", address);

  ASSERT(address >= 0 && (size_t)address < vm->code_size);

  PROPAGATE_ERROR(vm_push_im32(vm, (Number)vm->IP));

  vm->IP = address;

  return 0;
}

int vm_call_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  Number address;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &address));

  DEBUG_PRINT("CALL R%d\n", reg);

  ASSERT(address >= 0 && (size_t)address < vm->code_size);

  PROPAGATE_ERROR(vm_push_im32(vm, (Number)vm->IP));

  vm->IP = address;

  return 0;
}

int vm_ret(VM *vm) {
  DEBUG_PRINT("RET\n");

  Number address;
  PROPAGATE_ERROR(vm_pop_im32(vm, &address));

  ASSERT(address >= 0 && (size_t)address < vm->code_size);

  vm->IP = address;

  return 0;
}
