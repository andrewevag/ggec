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
int16_t x[2];
void main(){
	int16_t *p, q;
	bool f;
	x[0] = 0;
	p = &x[0];
	(*p)++;
	writeInteger(*p);
	writeChar('\n');
	
	*p = - (*p);
	writeInteger(*p);
	writeChar('\n');

	*p = + (*p);
	writeInteger(*p);
	writeChar('\n');
	f = ! true;
	writeInteger((int16_t) f);
	writeChar('\n');
}

