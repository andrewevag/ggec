#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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

bool prime(int16_t n)
   {
      int16_t i;

      if (n < 0)
         return prime(-n);
      if (n < 2)
         return false;
      if (n == 2)
         return true;
      if (n % 2 == 0)
         return false;
      for (i = 3; i <= n / 2; i += 2)
         if (n % i == 0)
            return false;
      return true;
   }


int main () 
{
   int16_t limit, number, counter;

   

   writeString("Please, give the upper limit: ");
   limit = readInteger();
   writeString("Prime numbers between 0 and ");
   writeInteger(limit);
   writeString("\n\n");
   counter = 0;
   if (limit >= 2) {
      counter++;
      writeString("2\n");
   }
   if (limit >= 3) {
      counter++;
      writeString("3\n");
   }
   for (number = 6; number <= limit; number += 6) {
      if(prime(number - 1)) {
         counter++;
         writeInteger(number - 1);
         writeString("\n");
      }
      if(number != limit && prime(number + 1)) {
         counter++;
         writeInteger(number + 1);
         writeString("\n");
      }
   }
   writeString("\n");
   writeInteger(counter);
   writeString(" prime number(s) were found.\n");
   return 0;
}
