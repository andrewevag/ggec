#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void writeString(char * str){
	printf("%s", str);
}
void writeInteger(int16_t i){
	printf("%hd", i);
}



void swap(int16_t* x, int16_t* y)
{
		int16_t t;
		t = *x;
		*x = *y;
		*y = t;
}
void bsort(int16_t * x, int16_t n)
{
	int16_t i;
	bool changed;
	

	for(changed = true; changed;)
		for(i = 0, changed = false; i < n-1; i++)
			if(x[i] > x[i+1]){
				swap(&x[i], &x[i+1]);
				changed = true;
			}
}

void printArray(char * msg, int16_t * x, int16_t n)
{
		int16_t i;
		writeString(msg);
		for(i = 0; i < n; i++) {
			if(i > 0)
				writeString(", ");
			writeInteger(x[i]);
		}
		writeString("\n");
}
int main()
{
	

	int16_t i, x[16], seed;
	for(i = 0, seed = 65; i < 16; i++)
		x[i] = seed = (seed * 137 + 221 + i) % 101;
	printArray("Initial array: ", x, 16);
	bsort(x, 16);
	printArray("Sorted array: ",x, 16);
	return 0;
}