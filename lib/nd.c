#include <stdio.h>
#include <string.h>
#include <stdint.h>
void writeString_char_ptr(char* s){
	printf("%s", s);
}

int16_t strlen_char_ptr(char* s){
	return strlen(s);
}
