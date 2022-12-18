#include <stdint.h>
#include <math.h>
#include <string.h>
int16_t trunc_double (long double d){
	return truncf(d);
}
int16_t round_double (long double d){
	return round(d);
}
int16_t ord_char (char c){
	return c;
}
char chr_int (int16_t n){
	return n;
}