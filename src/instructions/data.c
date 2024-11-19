#include "data.h"
#include "common.h"

INSTRUCTION(mov_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Number, im32);
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
  READ(Number, value, vm->registers.S);
  WRITE(Number, value, vm->registers.D);
})

INSTRUCTION(load_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Number, address);
  DEBUG_PRINT_INSTRUCTION_IM("LOAD", reg, address);
  READ(Number, value, address);
  WRITE_REG(reg, value);
})

INSTRUCTION(load_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("LOAD", reg_d, reg_s);
  READ_REG(reg_s, address);
  READ(Number, value, address);
  WRITE_REG(reg_d, value);
})

INSTRUCTION(stor_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Number, address);
  DEBUG_PRINT_INSTRUCTION_IM("STOR", reg, address);
  WRITE(Number, val, address);
})

INSTRUCTION(stor_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("STOR", reg_d, reg_s);
  WRITE(Number, val_d, val_s);
})
