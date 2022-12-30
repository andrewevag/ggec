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


void tester(int16_t *n){
	void incrementer(){
		void doubleincrementer(){
			*n *= 2 + (*n % 4);
		}
		doubleincrementer();
		(*n) ++;
		doubleincrementer();
		
	}
	incrementer();
	incrementer();
} 

char s[10];
void dosomething(){
	int16_t dosomething1(int16_t x){
		writeInteger(x);
		writeString("\n");
		return x;
	}
	int16_t dosomething2(char c){
		s[3] = c;
		return 0;
	}
	dosomething1(12);
	dosomething2('c');
	writeChar(s[3]);
	writeChar('\n');
	
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
	tester(&y);
    writeInteger(y);
	writeString("\n");
	dosomething();
}

