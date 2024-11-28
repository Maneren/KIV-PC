#include "vm.h"
#include "defs.h"
#include "instructions/all.h"
#include "instructions/common.h"
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
    return EXIT_ARGS;
  }

  if (output_filepath) {
    vm->output = fopen(output_filepath, "w");

    if (!vm->output) {
      fprintf(stderr, "Failed to open output file: '%s'\n", output_filepath);
      return EXIT_ARGS;
    }
  } else {
    vm->output = stdout;
  }

  char header[KMX_HEADER_SIZE + 1];
  header[KMX_HEADER_SIZE] = '\0';

  if (fread(header, sizeof(char), KMX_HEADER_SIZE, input) != KMX_HEADER_SIZE) {
    fprintf(stderr, "Failed to read header: '%s'\n", input_filepath);
    return EXIT_FILE;
  }

  if (strncmp(header, KMX_HEADER, KMX_HEADER_SIZE) != 0) {
    fprintf(stderr, "Invalid KMX file header: '%s'\n", input_filepath);
    return EXIT_FILE;
  }

  int data_size_header = 0;

  if (fread(&data_size_header, sizeof(int), 1, input) != 1) {
    fprintf(stderr, "Failed to read data size: '%s'\n", input_filepath);
    return EXIT_FILE;
  }

  size_t data_size = (size_t)data_size_header;

  SAFE_ALLOCATE(vm->data_segment, data_size, sizeof(Byte));
  vm->data_size = data_size;

  if (fread(vm->data_segment, sizeof(Byte), data_size, input) != data_size) {
    fprintf(stderr, "Failed to read data segment: '%s'\n", input_filepath);
    return EXIT_FILE;
  }

  SAFE_ALLOCATE(vm->code_segment, VM_MEMORY_SIZE, sizeof(Byte));

  size_t code_size =
      fread(vm->code_segment, sizeof(Byte), VM_MEMORY_SIZE, input);

  if (!feof(input)) {
    fprintf(stderr, "Failed to read code segment: '%s'\n", input_filepath);
    return EXIT_FILE;
  }

  vm->code_size = code_size;

  fclose(input);

  SAFE_ALLOCATE(vm->error_msg, VM_ERROR_BUFFER_SIZE, sizeof(char));

  return 0;
}

/**
 * @brief Run the VM
 *
 * @param vm The VM
 * @return int Exit code
 *
 * Will run the VM from start of the code segment until HALT is encountered or
 * the end of the code segment is reached. The exit code can be EXIT_SUCCESS or
 * any of the error codes defined in defs.h.
 */
int vm_run(VM *vm) {
  clock_t start_time = clock();
  int halted = 0;

  // NOTE: IP is incremented in the loop when reading the next instruction or
  // it's arguments. Expected exit from the loop is when HALT is encountered,
  // this condition is only a fallback for malformed code without HALT.
  for (vm->IP = 0, vm->instructions_count = 0;
       !halted && vm->IP < vm->code_size;) {
    if (vm->debug == DEBUG_MEMORY)
      vm_print(vm);

    Byte instruction = vm->code_segment[vm->IP];

    vm->IP++;
    vm->instructions_count++;

    if (instruction == 0x00) { // HALT
      halted = 1;
      break;
    }

    instruction_handler_t handler = instruction_table[instruction];

    if (!handler) {
      snprintf(vm->error_msg, VM_ERROR_BUFFER_SIZE,
               "Unknown instruction 0x%02X at address 0x%08lX\n", instruction,
               vm->IP);

      return EXIT_UNKNOWN;
    }

    TRY(handler(vm));
  }

  if (!halted) {
    printf("WARNING: Reached EOF without HALT...\n");
  } else {
    DEBUG_PRINT("HALT encountered...\n");
  }

  if (vm->debug > DEBUG_NONE) {
    printf("Execution time: %.3f seconds (%zu instructions)\n",
           (float)(clock() - start_time) / CLOCKS_PER_SEC,
           vm->instructions_count);
    printf("\nFinal state:\n");
    vm_print(vm);
  }

  return EXIT_SUCCESS;
}

/**
 * @brief Print the header of a data table
 */
void print_header(void) {
  printf("      ");

  for (int i = 0; i < 16; i++) {
    if (i % 4 == 0)
      printf(" ");

    printf("%02X ", i);
  }

  printf("\n");
}

/**
 * @brief Pretty print a line of a data table
 *
 * @param index Current index
 * @param data The data
 * @param size Size of the data in bytes
 */
void print_data_line(size_t index, const Byte *data, size_t size) {
  printf("%04lX:", index);
  for (size_t j = 0; j < 16; j++) {
    if (j % 4 == 0) {
      printf(" ");
    }

    if (index + j >= size) {
      printf(" 00");
      continue;
    }

    printf(" %02hhX", data[index + j]);
  }
  printf("\n");
}

/**
 * @brief Pretty print a data table to hexadecimal
 *
 * @param data The data
 * @param size Size of the data in bytes
 */
void pretty_print_data(const Byte *data, size_t size) {
  print_header();
  for (size_t i = 0; i < size; i += 16) {
    print_data_line(i, data, size);
  }
}

/**
 * @brief Pretty print the state of the VM
 *
 * @param vm The VM
 */
void vm_print(const VM *vm) {
  printf("Registers:\n");
  printf("A:  0x%08X\n", vm->registers.A);
  printf("B:  0x%08X\n", vm->registers.B);
  printf("C:  0x%08X\n", vm->registers.C);
  printf("D:  0x%08X\n", vm->registers.D);
  printf("S:  0x%08X\n", vm->registers.S);
  printf("SP: 0x%08X\n", vm->registers.SP);
  printf("IP: 0x%08lX\n", vm->IP);
  printf("Flags: 0b" BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(vm->flags));

  printf("Data segment: [0x%lX]\n", vm->data_size);
  pretty_print_data(vm->data_segment, vm->data_size);
  printf("\n");
  printf("Code segment: [0x%lX]\n", vm->code_size);
  pretty_print_data(vm->code_segment, vm->code_size);
  printf("\n");
  printf("Stack segment: [0x%lX]\n", vm->stack_size);
  pretty_print_data(vm->stack_segment, (vm->stack_size / 0x10 + 1) * 0x10);
  printf("\n");
}

/**
 * @brief Free all memory allocated by the VM
 *
 * @param vm The VM
 */
void vm_free(VM *vm) {
  SAFE_FREE(&vm->data_segment);
  SAFE_FREE(&vm->code_segment);
  SAFE_FREE(&vm->stack_segment);
  if (vm->error_msg) {
    SAFE_FREE(&vm->error_msg);
  }
  if (vm->output) {
    fclose(vm->output);
  }
}
