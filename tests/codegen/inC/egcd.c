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
int16_t egcd(int16_t a, int16_t b, int16_t* x, int16_t * y){
	if (a == 0){
		*x = 0;
		*y = 1;
		return b;
	}

	int16_t x1, y1;
	int16_t gcd = egcd(b%a, a, &x1, &y1);

	*x = y1 - (b/a) * x1;
	*y = x1;
	return gcd;
}

void formatPrintEGCD(int16_t a, int16_t b){
	int16_t res;
	int16_t x, y;
	res = egcd(a, b, &x, &y);
	writeString("gcd(");
	writeInteger(a);
	writeString(", ");
	writeInteger(b);
	writeString(") = (");
	writeInteger(x);
	writeString(") * ");
	writeInteger(a);
	writeString(" + (");
	writeInteger(y);
	writeString(") * ");
	writeInteger(b);
	writeString(" = ");
	writeInteger(res);
	writeChar('\n');
}

void main(){
	int a, b, i, j;
	a = 13;
	b = 120;
	formatPrintEGCD(12, 152);
	formatPrintEGCD(a, b);
	formatPrintEGCD(130, 1919);
}
