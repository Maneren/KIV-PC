#include "data.h"
#include "common.h"

INSTRUCTION(mov_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Word, im32);
  DEBUG_PRINT_INSTRUCTION_IM("MOV", reg, im32);
  WRITE_REG(reg, im32);
})

INSTRUCTION(mov_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("MOV", reg_d, reg_s);
  WRITE_REG(reg_d, val_s);
})

INSTRUCTION(movsd, {
  DEBUG_PRINT("MOVSD\n");
  READ(Word, value, vm->registers.S);
  WRITE(Word, value, vm->registers.D);
})

INSTRUCTION(load_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Word, address);
  DEBUG_PRINT_INSTRUCTION_IM("LOAD", reg, address);
  READ(Word, value, address);
  WRITE_REG(reg, value);
})

INSTRUCTION(load_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("LOAD", reg_d, reg_s);
  READ_REG(reg_s, address);
  READ(Word, value, address);
  WRITE_REG(reg_d, value);
})

INSTRUCTION(stor_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Word, address);
  DEBUG_PRINT_INSTRUCTION_IM("STOR", reg, address);
  WRITE(Word, val, address);
})

INSTRUCTION(stor_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("STOR", reg_d, reg_s);
  WRITE(Word, val_d, val_s);
})

INSTRUCTION(outd_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("OUTD R%hhX\n", reg);
  fprintf(vm->output, "%d\n", val);
})

INSTRUCTION(outc_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("OUTC R%hhX\n", reg);
  fprintf(vm->output, "%c\n", (char)val);
})

INSTRUCTION(outs_reg, {
  READ_REG_ARG(reg, address);
  DEBUG_PRINT("OUTS R%hhX\n", reg);
  for (int ch; (ch = vm->data_segment[address++]);) {
    ASSERT(address >= 0 && (size_t)address < vm->data_size,
           "Read outside data segment from address 0x%08X", address);
    putc(ch, vm->output);
  }
})

INSTRUCTION(inpd_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("INPD R%hhX\n", reg);
  scanf("%d", &val);
  WRITE_REG(reg, val);
})

INSTRUCTION(inpc_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("INPC R%hhX\n", reg);
  ASSERT(scanf("%c", (char *)&val) == 1,
         "Unexpected end of file while reading character from stdin");
  WRITE_REG(reg, val);
})

INSTRUCTION(inps_reg, {
  READ_REG_ARG(reg, address);
  DEBUG_PRINT("INPS R%hhX\n", reg);
  for (int ch; (ch = getchar()) != '\n'; address++) {
    ASSERT(ch != EOF,
           "Unexpected end of file while reading character from stdin");
    ASSERT(address >= 0 && (size_t)address < vm->data_size,
           "Write outside data segment to address 0x%08X", address);
    vm->data_segment[address] = (char)ch;
  }
})
