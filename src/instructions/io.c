#include "io.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int vm_outd_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("OUTD R%d\n", reg);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  fprintf(vm->output, "%d\n", value);

  return EXIT_SUCCESS;
}

int vm_outc_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("OUTC R%d\n", reg);

  Number value;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &value));

  fprintf(vm->output, "%c\n", value);

  return EXIT_SUCCESS;
}

int vm_outs_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("OUTS R%d\n", reg);

  Number address;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &address));

  ASSERT(address >= 0 && (size_t)address < vm->data_size,
         "Read outside data segment from address 0x%08X", address);

  fprintf(vm->output, "%s\n", (const char *)vm->code_segment + address);

  return EXIT_SUCCESS;
}

int vm_inpd_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("INPD R%d\n", reg);

  Number value;
  scanf("%d", &value);

  return vm_set_reg(vm, reg, value);
}

int vm_inpc_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("INPC R%d\n", reg);

  int ch = getchar();
  ASSERT(ch != EOF,
         "Unexpected end of file while reading character from stdin");

  return vm_set_reg(vm, reg, ch & 0xFF);
}

int vm_inps_reg(VM *vm) {
  Byte reg;
  PROPAGATE_ERROR(vm_code_read_reg(vm, &reg));

  DEBUG_PRINT("INPS R%d\n", reg);

  Number address;
  PROPAGATE_ERROR(vm_get_reg(vm, reg, &address));

  for (int ch, i = 0; (ch = getchar()) != '\n'; i++) {
    ASSERT(ch != EOF,
           "Unexpected end of file while reading character from stdin");
    ASSERT(address >= 0 && (size_t)address + i + sizeof(char) <= vm->data_size,
           "Write outside data segment to address 0x%08X", address);
    vm->data_segment[address + i] = (char)ch;
  }

  return EXIT_SUCCESS;
}
