#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
void writeString(char * str){
        printf("%s", str);
}
void writeInteger(int16_t i){
        printf("%hd", i);
}



void main(){
    int16_t x;
    int16_t y;
    int16_t is2(){
		int16_t z[10];
		void printX(){
			writeInteger(x);
			writeString("\n");
			z[2]++;
		}
		z[2] = 1;
		printX();
        x = 10;
		printX();
		x = 20;
		printX();
		return z[2];
    }
    x = y = 2;
    writeInteger(is2());
	writeString("\n");
}

