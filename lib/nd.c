#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* _newF(int16_t size){
	return (char*) malloc(size);
}

void _deleteF(char* p){
	free(p);
}