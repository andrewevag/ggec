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
	int16_t res;
	int16_t **p;
	int16_t i, j, k;
	bool f, g, s;
	i = 100;
	res = 10000;
	p = (int16_t**) malloc(i * sizeof(int16_t*));
	for(j = 0; j < i; j++){
		p[j] = (int16_t*) malloc(i* sizeof(int16_t));
	}
	for(j = 0; j < i; j++){
		for(k = 0; k < i; k++){
			p[j][k] = j*i + k;
			writeString("p[");
			writeInteger(j);
			writeString("][");
			writeInteger(k);
			writeString("]=");
			writeIntegerLn(p[j][k]);
		}
	}
}
