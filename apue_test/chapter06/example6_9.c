#include<sys/utsname.h>
#include<stdlib.h>
#include<cstdio>
#include<unistd.h>
#include<time.h>
int main()
{
	time_t *t;
	time(&t);
	printf(" %ln",t);
	return 0;
}
