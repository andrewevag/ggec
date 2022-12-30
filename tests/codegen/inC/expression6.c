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
	bool f, g, h;
	int16_t z, x;
	long double* p;
	x = 2;
	z = 2;
	x = z++ + z;
	writeIntegerLn(x);
	x = ++z + z;
	writeIntegerLn(x);
}

