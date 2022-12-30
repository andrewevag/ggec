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

void main(){
	long double res;
	long double *p;
	int i;
	bool f, g, s;
	x[0] = 12.05;
	x[1] = 13.9432;
	f = x[0] > x[1];
	g = x[0] <= x[1];
	res = (x[0] > x[1]) ? (x[0] <= x[1] ? x[0] + x[1] : x[0] - x[1]) : x[0]*x[1];
	writeRealLn(res);
	res = x[0] > 0.0 ? x[0] * (-2.0) : x[0] * (2.0);
	writeRealLn(res);
}
