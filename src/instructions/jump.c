#include "jump.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int vm_jmp_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JMP 0x%04X\n", address);

  ASSERT(address >= 0 && (size_t)address < vm->code_size);

  vm->IP = address;

  return EXIT_SUCCESS;
}

int vm_jmp_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("JMP R%02hhX\n", reg);

  Number address;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &address));

  ASSERT(address >= 0 && (size_t)address < vm->code_size);

  vm->IP = address;

  return EXIT_SUCCESS;
}

int vm_je_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JE 0x%04X\n", address);

  if (vm->flags & FLAG_EQ) {
    size_t target = vm->IP + (size_t)address;
    ASSERT(target < vm->code_size);
    vm->IP = target;
  }

  return EXIT_SUCCESS;
}

int vm_jne_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JNE 0x%04X\n", address);

  if (!(vm->flags & FLAG_EQ)) {
    size_t target = vm->IP + (size_t)address;
    ASSERT(target < vm->code_size);
    vm->IP = target;
  }

  return EXIT_SUCCESS;
}

int vm_jg_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JG 0x%04X\n", address);

  if (vm->flags & FLAG_GT) {
    size_t target = vm->IP + (size_t)address;
    ASSERT(target < vm->code_size);
    vm->IP = target;
  }

  return EXIT_SUCCESS;
}

int vm_jge_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JGE 0x%04X\n", address);

  if (vm->flags & (FLAG_GT | FLAG_EQ)) {
    size_t target = vm->IP + (size_t)address;
    ASSERT(target < vm->code_size);
    vm->IP = target;
  }

  return EXIT_SUCCESS;
}

int vm_jng_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JNG 0x%04X\n", address);

  if (!(vm->flags & FLAG_GT)) {
    size_t target = vm->IP + (size_t)address;
    ASSERT(target < vm->code_size);
    vm->IP = target;
  }

  return EXIT_SUCCESS;
}

int vm_jl_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JL 0x%04X\n", address);

  if (vm->flags & FLAG_LT) {
    size_t target = vm->IP + (size_t)address;
    ASSERT(target < vm->code_size);
    vm->IP = target;
  }

  return EXIT_SUCCESS;
}

int vm_jle_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JLE 0x%04X\n", address);

  if (vm->flags & (FLAG_LT | FLAG_EQ)) {
    size_t target = vm->IP + (size_t)address;
    ASSERT(target < vm->code_size);
    vm->IP = target;
  }

  return EXIT_SUCCESS;
}

int vm_jnl_im32(VM *vm) {
  Number address;
  PROPAGATE_ERROR(vm_code_read_im32(vm, &address));

  DEBUG_PRINT("JNL 0x%04X\n", address);

  if (!(vm->flags & FLAG_LT)) {
    size_t target = vm->IP + (size_t)address;
    ASSERT(target < vm->code_size);
    vm->IP = target;
  }

  return EXIT_SUCCESS;
}
