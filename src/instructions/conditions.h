#include "../vm.h"

int vm_cmp_reg_im32(VM *vm);
int vm_cmp_reg_reg(VM *vm);

int vm_jmp_im32(VM *vm);
int vm_jmp_reg(VM *vm);
int vm_je_im32(VM *vm);
int vm_jne_im32(VM *vm);
int vm_jg_im32(VM *vm);
int vm_jge_im32(VM *vm);
int vm_jng_im32(VM *vm);
int vm_jl_im32(VM *vm);
int vm_jle_im32(VM *vm);
int vm_jnl_im32(VM *vm);
