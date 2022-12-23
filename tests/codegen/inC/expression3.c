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
int16_t x[2];
void writeIntegerLn(int16_t i){
	writeInteger(i);
	writeChar('\n');
}
void main(){
	int16_t res;
	bool f, g, h;
	x[0] = 12;
	x[1] = 13;
	h = true;
	g = false;
	res = x[0] + x[1];
	writeIntegerLn(res);
	res = x[0] * x[1];
	writeIntegerLn(res);
	res = x[0] / x[1];
	writeIntegerLn(res);
	res = x[0] % x[1];
	writeIntegerLn(res);
	res = x[0] - x[1];
	writeIntegerLn(res);
	f   = x[0] < x[1];
	writeIntegerLn((int16_t) f);
	f   = x[0] > x[1];
	writeIntegerLn((int16_t) f);
	f   = x[0] >= x[1];
	writeIntegerLn((int16_t) f);
	f   = x[0] <= x[1];
	writeIntegerLn((int16_t) f);
	f   = x[0] == x[1];
	writeIntegerLn((int16_t) f);
	f   = x[0] != x[1];
	writeIntegerLn((int16_t) f);
	f   = g && h;
	writeIntegerLn((int16_t) f);
	f   = g || h;
	writeIntegerLn((int16_t) f);
}

