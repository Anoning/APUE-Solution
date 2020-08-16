#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <syslog.h>
#include <error.c>

void daemonize(const char *cmd)
{
	int i,fd0,fd1,fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;


	/*
	 * step 1:clear file creation mask
	 */
	umask(0);

	/*
	 *step 2:get maximum number of file descriptors
	 */
	if(getrlimit(RLIMIT_NOFILE,&rl)<0)
		err_quit("%s: can't get file limit",cmd);
	/*
	 * step 3:session leader
	 */
	if((pid=fork())<0)
		err_quit("%s: can't fork",cmd);
	else if(pid!=0)
		exit(0);
	setsid();
	/*
	 * step 4:won't allocate controlling TTYs
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	if(sigaction(SIGHUP,&sa,NULL)<0)
		err_quit("%s : can't ignore SIGHUP",cmd);
	if((pid=fork())<0)
		err_quit("%s : can't fork",cmd);
	else if (pid!=0)
		exit(0);

	/*
	 * step 4: change the current working directory
	 */
	if(chdir("/")<0)
		err_quit("%s : can't change directory to /",cmd);

	/*
	 * step 5: close all open file descriptors
	 */

	if(rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for( i=0;i<rl.rlim_max;i++)
		close(i);

	/*
	 * step 6: attach fd 0, 1, 2 to /dev/null
	 */
	fd0=open("/dev/null",O_RDWR);
	fd1=dup(0);
	fd2=dup(0);

	/*
	 * step 7: init to log file
	 */
	openlog(cmd,LOG_CONS,LOG_DAEMON);
	if(fd0!=0||fd1!=1||fd2!=2)
		syslog(LOG_ERR,"unexpected file descriptors %d %d %d",fd0,fd1,fd2);
	exit(1);


}

int main() {
    FILE *fp;

    fp = fopen("daemonize-getlogin.log", "w");

    fprintf(fp, "login: %s\n", getlogin());
    daemonize("daemonize-getlogin");
    fprintf(fp, "login: %s\n", getlogin());
    return EXIT_SUCCESS;
}

