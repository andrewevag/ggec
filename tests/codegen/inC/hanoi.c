#include <stdio.h>
#include <stdint.h>

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
void move(char * source, char * target)
		{
			writeString("Move from ");
			writeString(source);
			writeString(" to ");
			writeString(target);
			writeString(".\n");
		}


void hanoi (char * source, char * target, char * auxiliary, int16_t rings)
	{
		

		if (rings >= 1) {
			hanoi(source, auxiliary, target, rings-1);
			move(source, target);
			hanoi(auxiliary, target, source, rings-1);
		}
	}



int main()
{
	int16_t numberOfRings;
	writeString("Please, give the number of rings: ");
	numberOfRings = readInteger();
	writeString("\nHere is the solution:\n\n");
	hanoi("left", "right", "middle", numberOfRings);
	return 0;
}