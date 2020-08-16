#include "apue.h"
#include "common.h"
#include <fcntl.h>


/*
 * the lock_reg definition in fcntl.h 
 */
int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;
	lock.l_type=type;
	lock.l_start=offset;
	lock.l_whence=whence;
	lock.l_len=len;
	return(fcntl(fd, cmd, &lock));
}

int main() {
    pid_t pid1, pid2;
    int fd;

    sys_chk(fd = open("file.lock", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE));
    sys_chk(write(fd, "abc", 3));

    sys_chk(pid1 = fork());
    if (pid1 == 0) {  // Child 1
        // Readlock
        sys_chk(read_lock(fd, 0, SEEK_SET, 1));
        printf("Child 1 got read lock.\n");
        sleep(5);
        printf("Child 1 exiting.\n");
        exit(EXIT_SUCCESS);
    } 

    sys_chk(pid2 = fork());
    if (pid2 == 0) {  // Child 2
        // Write lock
        sleep(1);
        printf("Child 2 trying to get write lock.\n");
        sys_chk(writew_lock(fd, 0, SEEK_SET, 1));
        printf("Child 2 got write lock.\n");
        sleep(5);
        printf("Child 2 exiting.\n");
        exit(EXIT_SUCCESS);
    } 

    // Parent
    sleep(2);
    // Another read lock
    sys_chk(read_lock(fd, 0, SEEK_SET, 1));
    printf("Parent got read lock.\n");
    sleep(5);
    printf("Parent exiting.\n");

    return EXIT_SUCCESS;
}
