#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

void main(){
	int16_t i;
	i = (int16_t) true;
	writeIntegerLn(i);
	i = (int16_t) false;
	writeIntegerLn(i);
	i = (int16_t) 'c';
	writeIntegerLn(i);
	i = (int16_t) 12.02;
	writeIntegerLn(i);
	i = (int16_t) NULL;
	writeIntegerLn(i);
}
