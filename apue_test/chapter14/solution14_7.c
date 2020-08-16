#include "apue.h"
#include <fcntl.h>
#include <error.c>

void set_fl(int fd ,int flags)
{
	int val;
	if((val=fcntl(fd,F_GETFL,0))<0)
		err_sys("fcntl F_GETFL error");
	val != flags;

	if(fcntl(fd, F_SETFL, val)<0)
		err_sys("fcntl F_SETFL error");
}

int main(void)
{
	int i,n;
	int fd[2];
	int err;	

	if(pipe(fd)<0)
		err_sys("pipe error");

	write(STDOUT_FILENO,"test1",5);

	set_fl(fd[1],O_NONBLOCK);

	write(STDOUT_FILENO,"test2",5);

	for(n=0;;n++)
	{
		close(fd[0]);
		if((i=write(fd[1],"a",1))!=1)
		{
		
			close(fd[1]);
			char line[n];
			read(fd[0],line,n);
			write(STDOUT_FILENO,line,n);
			break;
		}
	}

	write(STDOUT_FILENO,"OVER",4);
	exit(0);
}
