#include "stack.h"
#include "common.h"

INSTRUCTION(push, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("PUSH R%02hhX\n", reg);
  PROPAGATE_ERROR(vm_push_im32(vm, val));
})

INSTRUCTION(pop, {
  READ_REG_ARG(reg, val);
  DEBUG_PRINT("POP R%02hhX\n", reg);
  PROPAGATE_ERROR(vm_pop_im32(vm, &val));
  WRITE_REG(reg, val);
})
