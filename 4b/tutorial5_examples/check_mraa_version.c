#include <stdio.h>
#include <mraa.h>

int main(){
	printf("MRAA vers: %s\n", mraa_get_version());
	return 0;
}
