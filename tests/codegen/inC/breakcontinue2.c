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


void main(){
	int16_t i, j;
	int16_t n;

	for(i = 0; i < 11; i++){
		for(j = 0; j < 13; j++){
			if(i == 3 && j == 7 ){
				// break for2;
				goto forafter2;
			}
			if(i==4 && j == 6){
				// continue for2.
				goto forafter1;
			}
			if(i==5 && j == 7){
				// continue for1;
				goto forafter2;
			}
			if(i==8 && j == 9){
				// break for1;
				goto forafter3;
			}
			writeInteger(i);
			writeChar(' ');
			writeInteger(j);
			writeChar('\n');
			forafter1: ;
		}
		forafter2: ; 
	}
	forafter3: ;
}

