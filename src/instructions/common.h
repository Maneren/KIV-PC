#include "../vm.h"

#define PROPAGATE_ERROR(code)                                                  \
  {                                                                            \
    int status;                                                                \
    if ((status = (code)))                                                     \
      return status;                                                           \
  }

// #define PROPAGATE(code)                                                        \
//   code;                                                                        \
//   if (vm->error_msg)                                                           \
//     return 1;

#define ASSERT(condition)                                                      \
  if (!(condition)) {                                                          \
    vm->error_msg = ("Assertion failed: " #condition "\n");                    \
    return 1;                                                                  \
  }

#ifdef INSTRUCTIONS_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

int vm_set_reg(VM *vm, Byte reg, Number im32);
int vm_get_reg(VM *vm, Byte reg, Number *out);

int vm_code_read_reg(VM *vm, size_t ip_offset, Byte *out);
int vm_code_read_im32(VM *vm, size_t ip_offset, Number *out);

int vm_read_im32(VM *vm, Number address, Number *out);
int vm_write_im32(VM *vm, Number address, Number im32);
