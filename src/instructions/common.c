#include "common.h"
#include <stddef.h>
#include <stdlib.h>

int vm_set_reg(VM *vm, Byte reg, Number im32) {
  switch (reg) {
  case 0x01:
    vm->registers.A = im32;
    break;
  case 0x02:
    vm->registers.B = im32;
    break;
  case 0x03:
    vm->registers.C = im32;
    break;
  case 0x04:
    vm->registers.D = im32;
    break;
  case 0x05:
    vm->registers.S = im32;
    break;
  case 0x06:
    vm->registers.SP = im32;
    break;
  default:
    vm->error_msg = "Invalid register ID";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int vm_get_reg(VM *vm, Byte reg, Number *out) {
  switch (reg) {
  case 0x01:
    *out = vm->registers.A;
    break;
  case 0x02:
    *out = vm->registers.B;
    break;
  case 0x03:
    *out = vm->registers.C;
    break;
  case 0x04:
    *out = vm->registers.D;
    break;
  case 0x05:
    *out = vm->registers.S;
    break;
  case 0x06:
    *out = vm->registers.SP;
    break;
  default:
    vm->error_msg = "Invalid register ID";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int vm_push_im32(VM *vm, Number im32) {
  Number address = vm->registers.SP;
  vm->registers.SP += sizeof(Number);
  ASSERT(address >= 0 && (size_t)address + sizeof(Number) <= vm->stack_size,
         "Write outside stack segment to address 0x%08X", address);
  *(Number *)(vm->stack_segment + vm->registers.SP) = im32;
  return EXIT_SUCCESS;
}
int vm_pop_im32(VM *vm, Number *out) {
  ASSERT(vm->registers.SP >= 0 &&
             (size_t)vm->registers.SP + sizeof(Number) <= vm->stack_size,
         "Read outside stack segment from address 0x%08X", vm->registers.SP);
  *out = *(Number *)(vm->stack_segment + vm->registers.SP);
  vm->registers.SP -= sizeof(Number);
  return EXIT_SUCCESS;
}

int vm_nop(VM *vm) {
  DEBUG_PRINT("NOP\n");
  return 0;
}
