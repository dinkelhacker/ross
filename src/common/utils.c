#include "types.h"
uint atoi(char *string)
{
	uint res;
	while(*string)
	{
		res = res * 10 + (*string) - '0';
		string++;
	}
	return res;
}
