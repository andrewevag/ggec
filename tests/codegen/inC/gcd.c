#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
void writeString(char * str){
        printf("%s", str);
}
void writeInteger(int16_t i){
        printf("%hd", i);
}
void writeChar(char c){
		printf("%c", c);
}
void writeReal(long double d){
	printf("%Lf", d);
}
long double x[2];
void writeIntegerLn(int16_t i){
	writeInteger(i);
	writeChar('\n');
}
void writeRealLn(long double i){
	writeReal(i);
	writeChar('\n');
}
int16_t gcd(int16_t a, int16_t b){
	if ( a == 0)
		return b;
	else return gcd(b % a, a);
}

void main(){
	int a, b, i, j;
	a = 13;
	b = 120;
	writeIntegerLn(gcd(a, b));
	writeIntegerLn(gcd(11, 121));
	writeIntegerLn(gcd(8, 942));
}
