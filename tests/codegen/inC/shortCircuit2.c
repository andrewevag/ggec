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
	bool * p;
	a = 13;
	b = 120;

	f = true;
	g = false;
	p = (bool*) NULL;
	if (f || *p){
		writeString("Correctly Got here\n");
	}
	if (g && *p){
		writeString("Incorrect to get here\n");
	}
	
}
