#include <stdio.h>
#include <stdint.h>

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
        if (x == 2){
            return 1;
        }
        else return 0;
    }
    x = y = 2;
    writeInteger(is2());
}

