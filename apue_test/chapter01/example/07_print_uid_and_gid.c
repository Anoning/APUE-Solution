#include "apue.h"
#include<error.c>

int main(void)
{
	printf(" uid = %d , gid = %d",getuid(), getgid());
	exit(0);
}
