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
	long double res;
	long double *p;
	int16_t i, j;
	bool f, g, s;
	i = 100;
	res = 1.001;
	p = (long double*) malloc(i * sizeof(long double));
	for(j = 0; j < i; j++){
		p[j] = res * 2.0;
		res *= 2.0;
	}
	for(j = 0; j < i; j++){
		writeRealLn(p[j]);
	}
	free(p);
}
