#include "arithemtic.h"
#include "common.h"
#include <stdio.h>

int vm_add_reg_im32(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  Number im32 = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg) + sizeof(im32);

  DEBUG_PRINT("ADD R%d, %d\n", reg, im32);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value + im32);
}

int vm_add_reg_reg(VM *vm) {
  Byte reg1 = vm->code_segment[vm->IP + 1];
  Byte reg2 = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg1) + sizeof(reg2);

  DEBUG_PRINT("ADD R%d, R%d\n", reg1, reg2);

  Number value1;
  PROPAGATE_ERROR(vm_get_reg(vm, reg1, &value1));

  Number value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg2, &value2));

  return vm_set_reg(vm, reg1, value1 + value2);
}

int vm_sub_reg_im32(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  Number im32 = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg) + sizeof(im32);

  DEBUG_PRINT("SUB R%d, %d\n", reg, im32);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  return vm_set_reg(vm, reg, value - im32);
}

int vm_sub_reg_reg(VM *vm) {
  Byte reg1 = vm->code_segment[vm->IP + 1];
  Byte reg2 = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg1) + sizeof(reg2);

  DEBUG_PRINT("SUB R%d, R%d\n", reg1, reg2);

  Number value1;
  PROPAGATE_ERROR(vm_get_reg(vm, reg1, &value1));

  Number value2;
  PROPAGATE_ERROR(vm_get_reg(vm, reg2, &value2));

  return vm_set_reg(vm, reg1, value1 - value2);
}

int vm_mul_reg_im32(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  Number im32 = *(Number *)(vm->code_segment + vm->IP + 2);
  vm->IP += sizeof(im32);

  DEBUG_PRINT("MUL R%d, %d\n", reg, im32);

  Number value;
  if (vm_get_reg(vm, reg, &value) != 0)
    return -1;

  return vm_set_reg(vm, reg, value * im32);
}

int vm_mul_reg_reg(VM *vm) {
  Byte reg1 = vm->code_segment[vm->IP + 1];
  Byte reg2 = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg1) + sizeof(reg2);

  DEBUG_PRINT("MUL R%d, R%d\n", reg1, reg2);

  Number value1, value2;
  if (vm_get_reg(vm, reg1, &value1) != 0)
    return -1;
  if (vm_get_reg(vm, reg2, &value2) != 0)
    return -1;

  return vm_set_reg(vm, reg1, value1 * value2);
}

int vm_div_reg_im32(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  Number im32 = *(Number *)(vm->code_segment + vm->IP + 2);
  vm->IP += sizeof(im32);

  DEBUG_PRINT("DIV R%d, %d\n", reg, im32);

  Number value;
  if (vm_get_reg(vm, reg, &value) != 0)
    return -1;

  ASSERT(im32 != 0);

  return vm_set_reg(vm, reg, value / im32);
}

int vm_div_reg_reg(VM *vm) {
  Byte reg1 = vm->code_segment[vm->IP + 1];
  Byte reg2 = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg1) + sizeof(reg2);

  DEBUG_PRINT("DIV R%d, R%d\n", reg1, reg2);

  Number value1, value2;
  if (vm_get_reg(vm, reg1, &value1) != 0)
    return -1;
  if (vm_get_reg(vm, reg2, &value2) != 0)
    return -1;

  ASSERT(value2 != 0);

  return vm_set_reg(vm, reg1, value1 / value2);
}

int vm_inc_reg(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  vm->IP += sizeof(reg);

  DEBUG_PRINT("INC R%d\n", reg);

  Number value;
  if (vm_get_reg(vm, reg, &value) != 0)
    return -1;

  return vm_set_reg(vm, reg, value + 1);
}

int vm_dec_reg(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  vm->IP += sizeof(reg);

  DEBUG_PRINT("DEC R%d\n", reg);

  Number value;
  if (vm_get_reg(vm, reg, &value) != 0)
    return -1;

  return vm_set_reg(vm, reg, value - 1);
}
