#include"apue.h"
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
struct foo
{
	int a,b,c,d;
};
void printfoo(const char *s,struct foo *fp)
{
	fputs(s,stdout);
	printf("foo.a=%d\n",fp->a);
	printf("foo.b=%d\n",fp->b);
	printf("foo.c=%d\n",fp->c);
	printf("foo.d=%d\n",fp->d);

}

void *thr_fn1(void *arg)
{
	struct foo *fp;
	fp=(struct foo *)malloc(sizeof(struct foo));
	fp->a=1;
	fp->b=2;
	fp->c=3;
	fp->d=4;
	printfoo("thread:\n",fp);
	return ((void*)fp);

}
int main(void)
{
	int err;
	pthread_t tid1;
	struct foo *fp;
	err=pthread_create(&tid1,NULL,thr_fn1,NULL);
	if(err!=0) err_exit(err,"can't create thread 1");
	pthread_join(tid1,(void**)&fp);
	sleep(2);
	printfoo("parent:\n",fp);
	return 0;
}

