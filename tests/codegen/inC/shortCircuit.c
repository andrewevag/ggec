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
void writeBoolean(bool b){
	if (b)
		writeString("true");
	else writeString("false");
}

void writeBooleanLn(bool b){
	writeBoolean(b);
	writeChar('\n');
}

void main(){
	int16_t a, b, i, j;
	bool f, g, h;
	a = 13;
	b = 120;

	f = true;
	g = false;

	// this should be 0
	a = ((f ? true : false) || (g ? true : false)) ? 1 : 0;
	writeIntegerLn(a);

	// another one
	b = ((f ? true : false) && (g ? true : false)) ? 100 : 0;
	writeIntegerLn(b);

	h = true && true;
	writeBooleanLn(h);
	h = true && false;
	writeBooleanLn(h);
	h = false && true;
	writeBooleanLn(h);
	h = false && false;
	writeBooleanLn(h);

	h = true || true;
	writeBooleanLn(h);
	h = true || false;
	writeBooleanLn(h);
	h = false || true;
	writeBooleanLn(h);
	h = false || false;
	writeBooleanLn(h);

}
