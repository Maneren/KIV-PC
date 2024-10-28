#include "data.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int vm_mov_reg_im32(VM *vm) {
  Byte reg;
  Number im32;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, &im32));

  DEBUG_PRINT("MOV R%d, %d\n", reg, im32);

  return vm_set_reg(vm, reg, im32);
}

int vm_mov_reg_reg(VM *vm) {
  Byte reg_d, reg_s;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg_d));
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg_s));

  DEBUG_PRINT("MOV R%d, R%d\n", reg_d, reg_s);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &value));

  return vm_set_reg(vm, reg_d, value);
}

int vm_movsd(VM *vm) {
  Number source = vm->registers.S;
  Number destination = vm->registers.D;

  DEBUG_PRINT("MOVSD\n");

  Number value;
  PROPAGATE_ERROR(vm_read_im32(vm, source, &value));
  PROPAGATE_ERROR(vm_write_im32(vm, destination, value))

  return EXIT_SUCCESS;
}

int vm_load_reg_im32(VM *vm) {
  Byte reg;
  Number address;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("LOAD R%d, %d\n", reg, address);

  Number value;
  PROPAGATE_ERROR(vm_read_im32(vm, address, &value));

  return vm_set_reg(vm, reg, value);
}

int vm_load_reg_reg(VM *vm) {
  Byte reg_d, reg_s;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg_d));
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg_s));

  DEBUG_PRINT("LOAD R%d, R%d\n", reg_d, reg_s);

  Number address;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &address));

  Number value;
  PROPAGATE_ERROR(vm_read_im32(vm, address, &value));

  return vm_set_reg(vm, reg_d, value);
}

int vm_stor_reg_im32(VM *vm) {
  Byte reg;
  Number address;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("STOR R%d, %d\n", reg, address);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  PROPAGATE_ERROR(vm_write_im32(vm, address, value))

  return EXIT_SUCCESS;
}

int vm_stor_reg_reg(VM *vm) {
  Byte reg_s, reg_d;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg_s));
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg_d));

  DEBUG_PRINT("STOR R%d, R%d\n", reg_s, reg_d);

  Number address;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_d, &address));

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg_s, &value));

  PROPAGATE_ERROR(vm_write_im32(vm, address, value))

  return EXIT_SUCCESS;
}
