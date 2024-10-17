#ifndef INCLUDE_SEMESTRALKA_STRING_H_
#define INCLUDE_SEMESTRALKA_STRING_H_

#include <stddef.h>

typedef struct {
  size_t size;
  char *data;
} String;

String String_trim(const String *this, char ch);

String String_new(size_t size);

String String_from_cstr(char *cstr);

void String_free(String *string);

#endif // INCLUDE_SEMESTRALKA_STRING_H_
