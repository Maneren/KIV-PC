#include "cmp.h"
#include "common.h"
#include <stdio.h>

unsigned char cmp_flags(Number a, Number b) {
  return ((a == b) * FLAG_EQ) | ((a < b) * FLAG_LT) | ((a > b) * FLAG_GT);
}

int vm_cmp_reg_im32(VM *vm) {
  Byte reg;
  Number im32;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, &im32));

  DEBUG_PRINT("CMP R%d, %d\n", reg, im32);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  vm->flags = cmp_flags(value, im32);

  return 0;
}

int vm_cmp_reg_reg(VM *vm) {
  Byte reg1, reg2;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg1));
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg2));

  DEBUG_PRINT("CMP R%d, R%d\n", reg1, reg2);

  Number value1, value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg1, &value1));
  PROPAGATE_ERROR(vm_get_reg(vm, reg2, &value2));

  vm->flags = cmp_flags(value1, value2);

  return 0;
}
