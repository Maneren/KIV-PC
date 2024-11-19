#include "arithemtic.h"
#include "common.h"

BINARY_OPERATION_REG_REG(add, +)
BINARY_OPERATION_REG_REG(sub, -)
BINARY_OPERATION_REG_REG(mul, *)
BINARY_OPERATION_REG_REG(div, /)
BINARY_OPERATION_REG_IM32(add, +)
BINARY_OPERATION_REG_IM32(sub, -)
BINARY_OPERATION_REG_IM32(mul, *)
BINARY_OPERATION_REG_IM32(div, /)

INSTRUCTION(inc_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("INC R%d\n", reg);
  WRITE_REG(reg, val + 1);
})

INSTRUCTION(dec_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("DEC R%d\n", reg);
  WRITE_REG(reg, val - 1);
})
