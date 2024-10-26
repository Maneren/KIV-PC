#include "arithemtic.h"
#include "common.h"
#include <stdio.h>

int vm_add_reg_im32(VM *vm) {
  Byte reg;
  Number im32;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, vm->IP + 2, &im32));
  vm->IP += sizeof(reg) + sizeof(im32);

  DEBUG_PRINT("ADD R%d, %d\n", reg, im32);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value + im32);
}

int vm_add_reg_reg(VM *vm) {
  Byte reg_d, reg_s;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg_d));
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 2, &reg_s));
  vm->IP += sizeof(reg_d) + sizeof(reg_s);

  DEBUG_PRINT("ADD R%d, R%d\n", reg_d, reg_s);

  Number value1;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_d, &value1));

  Number value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &value2));

  return vm_set_reg(vm, reg_d, value1 + value2);
}

int vm_sub_reg_im32(VM *vm) {
  Byte reg;
  Number im32;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, vm->IP + 2, &im32));
  vm->IP += sizeof(reg) + sizeof(im32);

  DEBUG_PRINT("SUB R%d, %d\n", reg, im32);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value - im32);
}

int vm_sub_reg_reg(VM *vm) {
  Byte reg_d, reg_s;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg_d));
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 2, &reg_s));
  vm->IP += sizeof(reg_d) + sizeof(reg_s);

  DEBUG_PRINT("SUB R%d, R%d\n", reg_d, reg_s);

  Number value1;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_d, &value1));

  Number value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &value2));

  return vm_set_reg(vm, reg_d, value1 - value2);
}

int vm_mul_reg_im32(VM *vm) {
  Byte reg;
  Number im32;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, vm->IP + 2, &im32));
  vm->IP += sizeof(im32);

  DEBUG_PRINT("MUL R%d, %d\n", reg, im32);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value * im32);
}

int vm_mul_reg_reg(VM *vm) {
  Byte reg_d, reg_s;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg_d));
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 2, &reg_s));
  vm->IP += sizeof(reg_d) + sizeof(reg_s);

  DEBUG_PRINT("MUL R%d, R%d\n", reg_d, reg_s);

  Number value1, value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_d, &value1));
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &value2));

  return vm_set_reg(vm, reg_d, value1 * value2);
}

int vm_div_reg_im32(VM *vm) {
  Byte reg;
  Number im32;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, vm->IP + 2, &im32));
  vm->IP += sizeof(reg) + sizeof(im32);

  DEBUG_PRINT("DIV R%d, %d\n", reg, im32);

  ASSERT(im32 != 0);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value / im32);
}

int vm_div_reg_reg(VM *vm) {
  Byte reg_d, reg_s;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg_d));
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 2, &reg_s));
  vm->IP += sizeof(reg_d) + sizeof(reg_s);

  DEBUG_PRINT("DIV R%d, R%d\n", reg_d, reg_s);

  Number value1, value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_d, &value1));
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &value2));

  ASSERT(value2 != 0);

  return vm_set_reg(vm, reg_d, value1 / value2);
}

int vm_inc_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg));
  vm->IP += sizeof(reg);

  DEBUG_PRINT("INC R%d\n", reg);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value + 1);
}

int vm_dec_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, vm->IP + 1, &reg));
  vm->IP += sizeof(reg);

  DEBUG_PRINT("DEC R%d\n", reg);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value - 1);
}
