#include "cmp.h"
#include "common.h"
#include <stdio.h>

int cmp_flags(Number a, Number b) {
  int flags = 0;

  if (a == b)
    flags |= FLAG_EQ;
  if (a > b)
    flags |= FLAG_GT;
  if (a < b)
    flags |= FLAG_LT;
  if (a < 0)
    flags |= FLAG_NEG;
  /* if (a == 0) */
  /*   flags |= FLAG_ZERO; */

  return flags;
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
