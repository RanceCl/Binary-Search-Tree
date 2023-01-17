
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

int TreeIPCount(int base, int count);


int main(void)
{
	int finalcount, base = 4;

	finalcount = TreeIPCount(base, 0);

	printf("\nFinalCount: %d\n\n", finalcount);


	return 0;
}



int TreeIPCount(int base, int count)
{
	int PrevCount = count+1;
	if(base == 0)
		return 0;
	else
	{
		printf("base: %d    +%d\n", base, PrevCount);
		count = count + TreeIPCount(base-1, PrevCount);
	}
	return count;
}


