#include <stdint.h>
#include <string.h>
int16_t strlen_char_ptr (char * s){
	return strlen(s);
}
int16_t strcmp_char_ptr_char_ptr (char * s1, char * s2){
	return strcmp(s1, s2);
}
void strcpy_char_ptr_char_ptr (char * trg, char * src){
	strcpy(trg, src);
}
void strcat_char_ptr_char_ptr (char * trg, char * src){
	strcat(trg, src);
}