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
	x[1] = 28;
    writeInteger(28);
	writeString("\n");
}

