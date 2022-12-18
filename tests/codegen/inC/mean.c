#include <stdint.h>
#include <stdio.h>


void writeString(char * str){
	printf("%s", str);
}
void writeInteger(int16_t i){
	printf("%hd", i);
}

int16_t readInteger(){
	int16_t i;
	scanf("%hd", &i);
	return i;
}
void writeReal(long double d){
    printf("%Lf", d);
}

int main ()
{

    int16_t    n, k, i, seed;
    long double sum;

    writeString("Give n: ");
    n = readInteger();
    writeString("Give k: ");
    k = readInteger();

    for (i = 0, sum = 0.0, seed = 65; i < k; i++)
        sum += (double) (seed = (seed * 137 + 221 + i) % n);
    
    if(k > 0) {
        writeString("Mean: ");
        writeReal(sum / (double) k);
        writeString("\n");
    }
    return 0;
}