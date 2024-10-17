#include "string.h"
#include <stdlib.h>
#include <string.h>

String String_trim(const String *this, char ch) {
  if (ch == 0)
    ch = ' ';

  size_t start = 0;
  while (this->data[start] == ch)
    start++;

  size_t end = this->size - 1;
  while (this->data[end] == ch)
    end--;

  String result = {end - start + 1, this->data + start};

  return result;
}

String String_new(size_t size) {
  String string = {size, malloc(size)};
  return string;
}

String String_from_cstr(char *cstr) {
  String string = {strlen(cstr), cstr};
  return string;
}

void String_free(String *string) {
  free(string->data);
  free(string);
}
