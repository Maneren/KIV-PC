#include "data.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int vm_mov_reg_im32(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  Number im32 = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg) + sizeof(im32);

  DEBUG_PRINT("MOV R%d, %d\n", reg, im32);

  return vm_set_reg(vm, reg, im32);
}

int vm_mov_reg_reg(VM *vm) {
  Byte reg_d = vm->code_segment[vm->IP + 1];
  Byte reg_s = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg_d) + sizeof(reg_s);

  DEBUG_PRINT("MOV R%d, R%d\n", reg_d, reg_s);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &value));

  return vm_set_reg(vm, reg_d, value);
}

int vm_movsd(VM *vm) {
  Number source = vm->registers.S;
  Number destination = vm->registers.D;

  DEBUG_PRINT("MOVSD\n");

  ASSERT(source >= 0 && (size_t)source < vm->data_size);
  ASSERT(destination >= 0 && (size_t)destination < vm->data_size);

  vm->data_segment[destination] = vm->data_segment[source];

  return EXIT_SUCCESS;
}

int vm_load_reg_im32(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  Number address = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg) + sizeof(address);

  DEBUG_PRINT("LOAD R%d, %d\n", reg, address);

  Number value;
  PROPAGATE_ERROR(vm_read_im32(vm, address, &value));

  return vm_set_reg(vm, reg, value);
}

int vm_load_reg_reg(VM *vm) {
  Byte reg_d = vm->code_segment[vm->IP + 1];
  Byte reg_s = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg_d) + sizeof(reg_s);

  DEBUG_PRINT("LOAD R%d, R%d\n", reg_d, reg_s);

  Number address;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &address));

  ASSERT(address >= 0 && (size_t)address < vm->data_size);

  Number value;
  PROPAGATE_ERROR(vm_read_im32(vm, address, &value));

  return vm_set_reg(vm, reg_d, value);
}

int vm_stor_reg_im32(VM *vm) {
  Byte reg = vm->code_segment[vm->IP + 1];
  Number address = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg) + sizeof(address);

  DEBUG_PRINT("STOR R%d, %d\n", reg, address);

  ASSERT(address >= 0 && (size_t)address < vm->data_size);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  *(Number *)(vm->data_segment + address) = value;

  return EXIT_SUCCESS;
}

int vm_stor_reg_reg(VM *vm) {
  Byte reg_d = vm->code_segment[vm->IP + 1];
  Byte reg_s = vm->code_segment[vm->IP + 2];
  vm->IP += sizeof(reg_d) + sizeof(reg_s);

  DEBUG_PRINT("STOR R%d, R%d\n", reg_d, reg_s);

  Number address;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_d, &address));

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &value));

  *(Number *)(vm->data_segment + address) = value;

  return EXIT_SUCCESS;
}
