#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
void writeString(char * str){
        printf("%s", str);
}
void writeInteger(int16_t i){
        printf("%hd", i);
}
void writeChar(char c){
		printf("%c", c);
}
char x[100];
char* ys;
void main(){
	int16_t i;
	ys = "Hello World!\n";
	for(i = 0; i < strlen(ys); i++){
		x[i] = ys[i];
	}
	x[i] = (char) 0;
	writeString(x);
}

