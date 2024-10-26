#include "../vm.h"

int vm_add_reg_im32(VM *vm);
int vm_add_reg_reg(VM *vm);
int vm_sub_reg_im32(VM *vm);
int vm_sub_reg_reg(VM *vm);
int vm_mul_reg_im32(VM *vm);
int vm_mul_reg_reg(VM *vm);
int vm_div_reg_im32(VM *vm);
int vm_div_reg_reg(VM *vm);
int vm_inc_reg(VM *vm);
int vm_dec_reg(VM *vm);
