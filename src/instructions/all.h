#include "call.h"
#include "common.h"
#include "conditions.h"
#include "data.h"
#include "operators.h"

typedef int (*instruction_handler_t)(VM *vm);

static instruction_handler_t instruction_table[256] = {
    // Data instructions
    [0x10] = vm_mov_reg_im32,  // MOV reg, im32
    [0x11] = vm_mov_reg_reg,   // MOV regd, regs
    [0x12] = vm_movsd,         // MOVSD
    [0x13] = vm_load_reg_im32, // LOAD reg, im32
    [0x14] = vm_load_reg_reg,  // LOAD regd, regs
    [0x15] = vm_stor_reg_im32, // STOR reg, im32
    [0x16] = vm_stor_reg_reg,  // STOR regd, regs

    // Stack instructions
    [0x20] = vm_push, // PUSH reg
    [0x21] = vm_pop,  // POP reg

    // Arithmetic instructions
    [0x30] = vm_add_reg_im32, // ADD reg, im32
    [0x31] = vm_add_reg_reg,  // ADD regd, regs
    [0x32] = vm_sub_reg_im32, // SUB reg, im32
    [0x33] = vm_sub_reg_reg,  // SUB regd, regs
    [0x34] = vm_mul_reg_im32, // MUL reg, im32
    [0x35] = vm_mul_reg_reg,  // MUL regd, regs
    [0x36] = vm_div_reg_im32, // DIV reg, im32
    [0x37] = vm_div_reg_reg,  // DIV regd, regs
    [0x38] = vm_inc_reg,      // INC reg
    [0x39] = vm_dec_reg,      // DEC reg

    // Logic instructions
    [0x40] = vm_and_reg_im32, // AND reg, im32
    [0x41] = vm_and_reg_reg,  // AND regd, regs
    [0x42] = vm_or_reg_im32,  // OR reg, im32
    [0x43] = vm_or_reg_reg,   // OR regd, regs
    [0x44] = vm_xor_reg_im32, // XOR reg, im32
    [0x45] = vm_xor_reg_reg,  // XOR regd, regs
    [0x46] = vm_not_reg,      // NOT reg

    // Bitshift instructions
    [0x50] = vm_shl_reg_im32, // SHL reg, im32
    [0x51] = vm_shl_reg_reg,  // SHL regd, regs
    [0x52] = vm_shr_reg_im32, // SHR reg, im32
    [0x53] = vm_shr_reg_reg,  // SHR regd, regs

    // Compare instructions
    [0x60] = vm_cmp_reg_im32, // CMP reg, im32
    [0x61] = vm_cmp_reg_reg,  // CMP regd, regs

    // Jump instructions
    [0x70] = vm_jmp_im32, // JMP im32
    [0x71] = vm_jmp_reg,  // JMP reg
    [0x72] = vm_je_im32,  // JE im32
    [0x73] = vm_jne_im32, // JNE im32
    [0x74] = vm_jg_im32,  // JG im32
    [0x75] = vm_jge_im32, // JGE im32
    [0x76] = vm_jng_im32, // JNG im32
    [0x77] = vm_jl_im32,  // JL im32
    [0x78] = vm_jle_im32, // JLE im32
    [0x79] = vm_jnl_im32, // JNL im32

    // Subprogram instructions
    [0x80] = vm_call_im32, // CALL im32
    [0x81] = vm_call_reg,  // CALL reg
    [0x82] = vm_ret,       // RET

    // Miscellaneous
    [0x90] = vm_nop, // NOP

    // IO instructions
    [0xF0] = vm_outd_reg, // OUTD reg
    [0xF1] = vm_outc_reg, // OUTC reg
    [0xF2] = vm_outs_reg, // OUTS reg
    [0xF3] = vm_inpd_reg, // INPD reg
    [0xF4] = vm_inpc_reg, // INPC reg
    [0xF5] = vm_inps_reg, // INPS reg
};
