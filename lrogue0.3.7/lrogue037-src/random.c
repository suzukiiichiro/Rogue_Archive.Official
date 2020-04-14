#include <stdlib.h>
#include "random.h"

int get_rand(int x, int y)
{
	return (rand() % ((1+y)-x) + x);
}

int rand_percent(int percentage)
{
	return(get_rand(1, 100) <= percentage);
}

int coin_toss(void)
{
	return get_rand(0,1);
}
