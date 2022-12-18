#include <stdint.h>
#include <string.h>
#include <stdio.h>

void writeString(char* str){
	printf("%s", str);
}



void reverse (char * s, char * r)
	{
		int i, l;

		for (i = 0, l = strlen(s); i < l; i++)
			r[i] = s[l-i-1];
		r[i] = '\0';
	}

int main ()
{
	

	char p [20];

	reverse("\n!dlrow olleH", p);
	writeString(p);
	return 0;
}
