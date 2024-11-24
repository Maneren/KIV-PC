#include "operators.h"
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

BINARY_OPERATION_REG_REG(shl, <<)
BINARY_OPERATION_REG_REG(shr, >>)
BINARY_OPERATION_REG_IM32(shl, <<)
BINARY_OPERATION_REG_IM32(shr, >>)

BINARY_OPERATION_REG_IM32(and, &&)
BINARY_OPERATION_REG_REG(and, &&)

BINARY_OPERATION_REG_IM32(or, ||)
BINARY_OPERATION_REG_REG(or, ||)

BINARY_OPERATION_REG_IM32(xor, !=)
BINARY_OPERATION_REG_REG(xor, !=)

INSTRUCTION(not_reg, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("NOT R%d\n", reg);
  WRITE_REG(reg, !val);
})