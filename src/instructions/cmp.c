#include "cmp.h"
#include "common.h"

unsigned char cmp_flags(Number a, Number b) {
  return ((a == b) * FLAG_EQ) | ((a < b) * FLAG_LT) | ((a > b) * FLAG_GT);
}

INSTRUCTION(cmp_reg_im32, {
  READ_REG_ARG(reg, val);
  READ_ARG(Number, im32);
  DEBUG_PRINT_INSTRUCTION_IM("CMP", reg, im32);
  vm->flags = cmp_flags(val, im32);
})

INSTRUCTION(cmp_reg_reg, {
  READ_REG_DS_PAIR(reg, val);
  DEBUG_PRINT_INSTRUCTION("CMP", reg_d, reg_s);
  vm->flags = cmp_flags(val_d, val_s);
})
