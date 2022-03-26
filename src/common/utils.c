#include <stdint.h>
uint32_t atoi(char *string)
{
	uint32_t res;
	while(*string)
	{
		res = res * 10 + (*string) - '0';
		string++;
	}
	return res;
}
