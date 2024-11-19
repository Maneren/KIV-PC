#include "../vm.h"

int vm_mov_reg_im32(VM *vm);
int vm_mov_reg_reg(VM *vm);
int vm_movsd(VM *vm);
int vm_load_reg_im32(VM *vm);
int vm_load_reg_reg(VM *vm);
int vm_stor_reg_im32(VM *vm);
int vm_stor_reg_reg(VM *vm);

int vm_outd_reg(VM *vm);
int vm_outc_reg(VM *vm);
int vm_outs_reg(VM *vm);

int vm_inpd_reg(VM *vm);
int vm_inpc_reg(VM *vm);
int vm_inps_reg(VM *vm);
