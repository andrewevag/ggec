
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

int16_t abs_int (int16_t n){
	return abs(n);
}
long double fabs_double (long double d){
	return fabsl(d);
}

long double sqrt_double (long double d){
	return sqrt(d);
}
long double sin_double (long double d){
	return sin(d);
}
long double cos_double (long double d){
	return cos(d);
}
long double tan_double (long double d){
	return tan(d);
}

long double atan_double (long double d){
	return atan(d);
}
long double exp_double (long double d){
	return exp(d);
}
long double ln_double (long double d){
	return log(d);
}
long double pi (){
	return M_PI;
}