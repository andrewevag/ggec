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
	int i;
	int j;
	hello: for(i = 0; i < 10; ++i){
		if(i % 2 == 0){
			continue; //aka continue hello;
		}
		writeInteger(i);
		writeChar('\n');
	}

	for(i = 9; i >= 0; i--){
		if(i % 2 == 1){
			continue;		
		}
		writeInteger(i);
		writeChar('\n');
	}
	bool f=false, g=false;

	for(i = 0; i < 10; i++){
		label1: if(f){
			if (++i >= 10) goto after;
			f = false;
		}
		 for(j = 0; j < 10; j++){
			label2: if(g){
				if(++j >= 10) goto after2;
				g = false;
			}
			
			writeInteger(i);
			writeChar(' ');
			writeInteger(j);
			writeChar('\n');
			if(j % 7 == 0 && j % 3 == 1 && i == 6){
				goto after; //aka break label1.
			}
			if(i % 3 == 2 && j % 3 == 2){
				goto after2; //aka break label2
			}
			if(i % 3 == 1 && j % 3 == 1){
				goto after2; //aka continue label1.

			}
			
		}
		after2: ;
	}
	after: ;
}

