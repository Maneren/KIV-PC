#include "../vm.h"

int vm_push(VM *vm);
int vm_pop(VM *vm);

int vm_call_im32(VM *vm);
int vm_call_reg(VM *vm);
int vm_ret(VM *vm);
