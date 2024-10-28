#include "vm.h"
#include "instructions/all.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                                   \
  ((byte) & 0x80 ? '1' : '0'), ((byte) & 0x40 ? '1' : '0'),                    \
      ((byte) & 0x20 ? '1' : '0'), ((byte) & 0x10 ? '1' : '0'),                \
      ((byte) & 0x08 ? '1' : '0'), ((byte) & 0x04 ? '1' : '0'),                \
      ((byte) & 0x02 ? '1' : '0'), ((byte) & 0x01 ? '1' : '0')

const char KMX_HEADER[3] = "KMX";
const size_t KMX_HEADER_SIZE = 3;

int init_vm_from_file(const char *input_filepath, const char *output_filepath,
                      VM *vm) {
  FILE *input = fopen(input_filepath, "r");

  if (!input) {
    fprintf(stderr, "Failed to open file: '%s'\n", input_filepath);
    return 1;
  }

  if (output_filepath) {
    vm->output = fopen(output_filepath, "w");

    if (!vm->output) {
      fprintf(stderr, "Failed to open output file: '%s'\n", output_filepath);
      return 1;
    }
  } else {
    vm->output = stdout;
  }

  char header[KMX_HEADER_SIZE + 1];
  header[KMX_HEADER_SIZE] = '\0';

  if (fread(header, sizeof(char), KMX_HEADER_SIZE, input) != KMX_HEADER_SIZE) {
    fprintf(stderr, "Failed to read header: '%s'\n", input_filepath);
    return 2;
  }

  if (strncmp(header, KMX_HEADER, 3) != 0) {
    fprintf(stderr, "Invalid KMX file header: '%s'\n", input_filepath);
    return 2;
  }

  int data_size_header = 0;

  if (fread(&data_size_header, sizeof(int), 1, input) != 1) {
    fprintf(stderr, "Failed to read data size: '%s'\n", input_filepath);
    return 2;
  }

  size_t data_size = (size_t)data_size_header;

  vm->data_segment = malloc(data_size);
  vm->data_size = data_size;

  if (fread(vm->data_segment, sizeof(Byte), data_size, input) != data_size) {
    fprintf(stderr, "Failed to read data segment: '%s'\n", input_filepath);
    return 2;
  }

  vm->code_segment = malloc(MEMORY_SIZE);

  size_t code_size = fread(vm->code_segment, sizeof(Byte), MEMORY_SIZE, input);

  if (!feof(input)) {
    fprintf(stderr, "Failed to read code segment: '%s'\n", input_filepath);
    return 2;
  }

  vm->code_size = code_size;

  fclose(input);

  return 0;
}

#define INSTRUCTION(code, handler)                                             \
  case code:                                                                   \
    if (handler(vm)) {                                                         \
      fprintf(                                                                 \
          stderr,                                                              \
          "Error in instruction 0x%02X at address 0x%08lX with reason:\n%s",   \
          code, vm->IP, vm->error_msg);                                        \
      return EXIT_FAILURE;                                                     \
    }                                                                          \
    break;

int vm_run(VM *vm) {

  clock_t start_time = clock();
  int halted = 0;

  // NOTE: jump instructions may modify the IP independently of this loop
  for (vm->IP = 0, vm->instructions_count = 0;
       !halted && vm->IP < vm->code_size;) {
    vm_print(vm);

    Byte instruction = vm->code_segment[vm->IP];

    printf("Instruction: 0x%02x @ 0x%04lX\n", instruction, vm->IP);

    vm->IP++;
    vm->instructions_count++;

    switch (instruction) {
    case 0x00: // HALT
      halted = 1;
      break;
    case 0x90: // NOP
      break;

      // DATA INSTRUCTIONS
      INSTRUCTION(0x10, vm_mov_reg_im32);  // MOV reg, im32
      INSTRUCTION(0x11, vm_mov_reg_reg);   // MOV regd, regs
      INSTRUCTION(0x12, vm_movsd);         // MOVSD
      INSTRUCTION(0x13, vm_load_reg_im32); // LOAD reg, im32
      INSTRUCTION(0x14, vm_load_reg_reg);  // LOAD regd, regs
      INSTRUCTION(0x15, vm_stor_reg_im32); // STOR reg, im32
      INSTRUCTION(0x16, vm_stor_reg_reg);  // STOR regd, regs

      // STACK INSTRUCTIONS
      INSTRUCTION(0x20, vm_push); // PUSH reg
      INSTRUCTION(0x21, vm_pop);  // POP reg

      // ARITHMETIC INSTRUCTIONS
      INSTRUCTION(0x30, vm_add_reg_im32); // ADD reg, im32
      INSTRUCTION(0x31, vm_add_reg_reg);  // ADD regd, regs
      INSTRUCTION(0x32, vm_sub_reg_im32); // SUB reg, im32
      INSTRUCTION(0x33, vm_sub_reg_reg);  // SUB regd, regs
      INSTRUCTION(0x34, vm_mul_reg_im32); // MUL reg, im32
      INSTRUCTION(0x35, vm_mul_reg_reg);  // MUL regd, regs
      INSTRUCTION(0x36, vm_div_reg_im32); // DIV reg, im32
      INSTRUCTION(0x37, vm_div_reg_reg);  // DIV regd, regs
      INSTRUCTION(0x38, vm_inc_reg);      // INC reg
      INSTRUCTION(0x39, vm_dec_reg);      // DEC reg

      // BITSHIFT INSTRUCTIONS
      INSTRUCTION(0x50, vm_shl_reg_im32); // SHL reg, im32
      INSTRUCTION(0x51, vm_shl_reg_reg);  // SHL regd, regs
      INSTRUCTION(0x52, vm_shr_reg_im32); // SHR reg, im32
      INSTRUCTION(0x53, vm_shr_reg_reg);  // SHR regd, regs

      // COMPARISON INSTRUCTIONS
      INSTRUCTION(0x60, vm_cmp_reg_im32); // CMP reg, im32
      INSTRUCTION(0x61, vm_cmp_reg_reg);  // CMP regd, regs

      // JUMP INSTRUCTIONS
      INSTRUCTION(0x70, vm_jmp_im32); // JMP im32
      INSTRUCTION(0x71, vm_jmp_reg);  // JMP reg
      INSTRUCTION(0x72, vm_je_im32);  // JE im32
      INSTRUCTION(0x73, vm_jne_im32); // JNE im32
      INSTRUCTION(0x74, vm_jg_im32);  // JG im32
      INSTRUCTION(0x75, vm_jge_im32); // JGE im32
      INSTRUCTION(0x76, vm_jng_im32); // JNG im32
      INSTRUCTION(0x77, vm_jl_im32);  // JL im32
      INSTRUCTION(0x78, vm_jle_im32); // JLE im32
      INSTRUCTION(0x79, vm_jnl_im32); // JNL im32

      // CALL INSTRUCTIONS
      INSTRUCTION(0x80, vm_call_im32); // CALL im32
      INSTRUCTION(0x81, vm_call_reg);  // CALL reg
      INSTRUCTION(0x82, vm_ret);       // RET

      // IO INSTRUCTIONS
      INSTRUCTION(0xF0, vm_outd_reg); // OUTD reg
      INSTRUCTION(0xF1, vm_outc_reg); // OUTC reg
      INSTRUCTION(0xF2, vm_outs_reg); // OUTS reg
      INSTRUCTION(0xF3, vm_inpd_reg); // INPD reg
      INSTRUCTION(0xF4, vm_inpc_reg); // INPC reg
      INSTRUCTION(0xF5, vm_inps_reg); // INPS reg
    default:
      fprintf(stderr, "Unknown instruction: 0x%02x\n", instruction);
      return EXIT_FAILURE;
    }

    if (instruction != 0x60 && instruction != 0x61)
      vm->flags = 0;
  }

  printf("Halt instruction encountered. Halting...\n");

  printf("Execution time: %.3f seconds (%zu instructions)\n",
         (double)(clock() - start_time) / CLOCKS_PER_SEC,
         vm->instructions_count);

  printf("\nFinal state:\n");
  vm_print(vm);

  return 0;
}

void print_header() {
  printf("       %02X %02X %02X %02X"
         "  %02X %02X %02X %02X"
         "  %02X %02X %02X %02X"
         "  %02X %02X %02X %02X\n",
         0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}

void print_data_line(size_t index, const Byte *data, size_t size) {
  printf("%04lX: ", index);
  for (size_t j = 0; j < 16; j++) {
    if (j % 4 == 0) {
      printf(" ");
    }

    Byte value = (index + j < size) ? data[index + j] : 0;
    printf("%02hhX ", value);
  }
  printf("\n");
}

void pretty_print_data(const Byte *data, size_t size) {
  print_header();
  for (size_t i = 0; i < size; i += 16) {
    print_data_line(i, data, size);
  }
}

void vm_print(const VM *vm) {
  printf("Registers:\n");
  printf("A:  0x%08X\n", vm->registers.A);
  printf("B:  0x%08X\n", vm->registers.B);
  printf("C:  0x%08X\n", vm->registers.C);
  printf("D:  0x%08X\n", vm->registers.D);
  printf("S:  0x%08X\n", vm->registers.S);
  printf("Sp: 0x%08X\n", vm->registers.SP);
  printf("IP: 0x%lX\n", vm->IP);
  printf("Flags: 0b" BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(vm->flags));

  printf("Data segment: (%lu)\n", vm->data_size);
  pretty_print_data(vm->data_segment, vm->data_size);
  printf("\n");
  printf("Code segment: (%lu)\n", vm->code_size);
  pretty_print_data(vm->code_segment, vm->code_size);
  printf("\n");
}

void vm_free(VM *vm) {
  free(vm->data_segment);
  free(vm->code_segment);
  fclose(vm->output);
}
