#include "shift.h"
#include "common.h"

BINARY_OPERATION_REG_REG(shl, <<)
BINARY_OPERATION_REG_REG(shr, >>)
BINARY_OPERATION_REG_IM32(shl, <<)
BINARY_OPERATION_REG_IM32(shr, >>)
