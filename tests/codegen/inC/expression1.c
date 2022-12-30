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

void writeIntegerLn(int16_t i){
	writeInteger(i);
	writeChar('\n');
}
void writeReal(long double d){
	printf("%Lf", d);
}
void main(){
	bool f, g;
	int16_t i, j, k;
	char * s;
	long double d;
	// Bool Constants	
	f = true;
	writeIntegerLn((int16_t)f);
	f = false;
	writeInteger((int16_t)f);
	// Int Constants
	i = 0;
	j = 123;
	k = 999;
	writeIntegerLn(i);
	writeIntegerLn(j);
	writeIntegerLn(k);
	s = NULL;
	d = 100.0e-6;
	writeReal(d);
	writeChar('\n');
	s = "Hello people\n";
	writeString(s);
}

