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
void writeReal(long double d){
	printf("%Lf", d);
}
long double x[2];
void writeIntegerLn(int16_t i){
	writeInteger(i);
	writeChar('\n');
}
void writeRealLn(long double i){
	writeReal(i);
	writeChar('\n');
}

void main(){
	long double res;
	bool f, g, h;
	int16_t z;
	long double* p;
	x[0] = 12.05;
	x[1] = 13.19;
	h = true;
	g = true;
	z =1212;
	z++;
	writeIntegerLn(--z);
	writeIntegerLn(++z);
	z--;
	writeIntegerLn(z);
	
	x[0]++;
	writeRealLn(x[0]--);
	writeRealLn(++x[1]);
	writeRealLn(++x[1]);
	p = x;
	p++;
	writeRealLn(*p);
	p--;
	writeRealLn(*p);
	--p;
	writeRealLn(*p);
	++p;
	writeRealLn(*p);

	f   = ++x[0] < x[1];
	writeIntegerLn((int16_t) f);
	f   = --x[0] > x[1]--;
	writeIntegerLn((int16_t) f);
	f   = x[0] >= x[1]++;
	writeIntegerLn((int16_t) f);
	f   = x[0] <= x[1];
	writeIntegerLn((int16_t) f);
	f   = x[0] == x[1];
	writeIntegerLn((int16_t) f);
	f   = x[0] != x[1];
	writeIntegerLn((int16_t) f);
	f   = g && h;
	writeIntegerLn((int16_t) f);
	f   = g || h;
	writeIntegerLn((int16_t) f);
}

