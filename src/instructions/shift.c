#include "shift.h"
#include "common.h"
#include <stdio.h>

int vm_shl_reg_im32(VM *vm) {
  Byte reg;
  Number im32;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, &im32));

  DEBUG_PRINT("SHL R%d, %d\n", reg, im32);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value << im32);
}

int vm_shl_reg_reg(VM *vm) {
  Byte reg1, reg2;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg1));
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg2));

  DEBUG_PRINT("SHL R%d, R%d\n", reg1, reg2);

  Number value1, value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg1, &value1));
  PROPAGATE_ERROR(vm_get_reg(vm, reg2, &value2));

  return vm_set_reg(vm, reg1, value1 << value2);
}

int vm_shr_reg_im32(VM *vm) {
  Byte reg;
  Number im32;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, &im32));

  DEBUG_PRINT("SHR R%d, %d\n", reg, im32);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value >> im32);
}

int vm_shr_reg_reg(VM *vm) {
  Byte reg1, reg2;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg1));
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg2));

  DEBUG_PRINT("SHR R%d, R%d\n", reg1, reg2);

  Number value1, value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg1, &value1));
  PROPAGATE_ERROR(vm_get_reg(vm, reg2, &value2));

  return vm_set_reg(vm, reg1, value1 >> value2);
}
