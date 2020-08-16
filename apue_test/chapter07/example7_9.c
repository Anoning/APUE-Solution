#include<stdlib.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
	if(argc!=2) perror("should enter one parameter");
	char *ptr=getenv(argv[1]);
	printf("%s",ptr);
	return 0;
}

