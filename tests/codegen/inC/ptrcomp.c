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
void writeIntegerLn(int16_t i){
	writeInteger(i);
	writeChar('\n');
}
void writeRealLn(long double i){
	writeReal(i);
	writeChar('\n');
}
void writeBoolean(bool b){
	if(b){
		printf("true");
	}
	else printf("false");
}

int16_t x[1000];
void main(){
	int16_t i;
	int16_t j;
	j = 50;
	for(int i =0 ; i < 1000; i+=25){
		writeBoolean(x + i > x + j);
		writeChar('\n');
		writeBoolean(x + i < x + j);
		writeChar('\n');
		writeBoolean(x + i >= x + j);
		writeChar('\n');
		writeBoolean(x + i <= x + j);
		writeChar('\n');
		writeBoolean(x + i == x + j);
		writeChar('\n');
		writeBoolean(x + i != x + j);
		writeChar('\n');
	}
}
