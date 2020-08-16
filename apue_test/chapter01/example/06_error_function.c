#include "apue.h"
#include <error.c>

int main(int argc, char *argv[])
{
	//strerror()
	fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
	errno=ENOENT;
	perror(argv[0]);
	exit(0);
}
