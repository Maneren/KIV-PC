#include "logic.h"
#include "common.h"

INSTRUCTION(and_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Number, im32);
  DEBUG_PRINT_INSTRUCTION_IM("AND", reg, im32);
  WRITE_REG(reg, val & im32);
})

INSTRUCTION(and_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("AND", reg_d, reg_s);
  WRITE_REG(reg_d, val_d & val_s);
})

INSTRUCTION(or_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Number, im32);
  DEBUG_PRINT_INSTRUCTION_IM("OR", reg, im32);
  WRITE_REG(reg, val | im32);
})

INSTRUCTION(or_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("OR", reg_d, reg_s);
  WRITE_REG(reg_d, val_d | val_s);
})

INSTRUCTION(xor_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Number, im32);
  DEBUG_PRINT_INSTRUCTION_IM("XOR", reg, im32);
  WRITE_REG(reg, val ^ im32);
})

INSTRUCTION(xor_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("XOR", reg_d, reg_s);
  WRITE_REG(reg_d, val_d ^ val_s);
})

INSTRUCTION(not_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("NOT R%d\n", reg);
  WRITE_REG(reg, ~val);
})
