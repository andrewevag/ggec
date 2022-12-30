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
void writeCharLn(char c){
	writeChar(c);
	writeChar('\n');
}
void writeBoolean(bool b){
	if(b){
		printf("true");
	}
	else printf("false");
}
void writeBooleanLn(bool b){
	writeBoolean(b);
	writeChar('\n');
}

void main(){
	char i;
	bool b;
	long double d;
	i = (char) true;
	writeCharLn(i);
	i = (char) false;
	writeCharLn(i);
	i = (char) 12;
	writeCharLn(i);
	i = (char) 12.02;
	writeCharLn(i);
	i = (char) NULL;
	writeCharLn(i);

	b = (bool) 10;
	writeBooleanLn(b);
	b = (bool) 0;
	writeBooleanLn(b);
	b = (bool) 'c';
	writeBooleanLn(b);
	b = (bool) 12.02;
	writeBooleanLn(b);
	b = (bool) NULL;
	writeBooleanLn(b);

	
	d = (long double) true;
	writeRealLn(d);
	d = (long double) false;
	writeRealLn(d);
	d = (long double) 'c';
	writeRealLn(d);
	d = (long double) 12;
	writeRealLn(d);
}
