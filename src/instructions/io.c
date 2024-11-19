#include "io.h"
#include "common.h"
#include <stdio.h>

INSTRUCTION(outd_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("OUTD R%02hhX\n", reg);
  fprintf(vm->output, "%d\n", val);
})

INSTRUCTION(outc_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("OUTC R%02hhX\n", reg);
  fprintf(vm->output, "%c\n", (char)val);
})

INSTRUCTION(outs_reg, {
  READ_REG_ARG(reg, address);
  DEBUG_PRINT("OUTS R%02hhX\n", reg);
  for (int ch, i = 0; (ch = vm->code_segment[address + i]); i++) {
    ASSERT(address >= 0 && (size_t)address < vm->data_size,
           "Read outside data segment from address 0x%08X", address);
    putc(ch, vm->output);
  }
})

INSTRUCTION(inpd_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("INPD R%02hhX\n", reg);
  scanf("%d", &val);
  WRITE_REG(reg, val);
})

INSTRUCTION(inpc_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("INPC R%02hhX\n", reg);
  ASSERT(scanf("%c", (char *)&val) == 1,
         "Unexpected end of file while reading character from stdin");
  WRITE_REG(reg, val);
})

INSTRUCTION(inps_reg, {
  READ_REG_ARG(reg, address);
  DEBUG_PRINT("INPS R%02hhX\n", reg);
  for (int ch; (ch = getchar()) != '\n'; address++) {
    ASSERT(ch != EOF,
           "Unexpected end of file while reading character from stdin");
    ASSERT(address >= 0 && (size_t)address + sizeof(char) <= vm->data_size,
           "Write outside data segment to address 0x%08X", address);
    vm->data_segment[address] = (char)ch;
  }
})
