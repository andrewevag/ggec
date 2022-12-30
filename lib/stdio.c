#include <stdio.h>
#include <stdint.h>


void writeInteger_int (int16_t n){
	printf("%d", n);
}
void writeBoolean_bool (char b){
	if(b == 0){
		printf("false");
	}else{
		printf("true");
	}	
}
void writeChar_char (char c){
	printf("%c", c);
}
void writeReal_double (long double d){
	printf("%Lf", d);
}
void writeString_char_ptr (char * s){
	printf("%s", s);
}

int16_t readInteger (){
	int16_t i;
	scanf("%hd", &i);
	return i;
}
char readBoolean () {
	int16_t i;
	scanf("%hd", &i);
	return i;
}
char readChar (){
	char c;
	scanf("%c", &c);
	return c;
}
long double readReal (){
	long double d;
	scanf("%Lf", &d);
	return d;
}
void readString_int_char_ptr (int16_t size, char * s){
	fgets(s, size, stdin);
}