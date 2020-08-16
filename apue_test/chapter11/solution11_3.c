#include"apue.h"
#include<stdbool.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<error.c>

struct job{
	struct job *j_next;
	struct job *j_prev;
	pthread_t j_id;
};

struct queue{
	struct job *q_head;
	struct job *q_tail;
	pthread_rwlock_t q_lock;
	pthread_cond_t qready;
	pthread_mutex_t cond_lock;
};

int queue_init(struct queue *qp)
{
	int err;
	err=pthread_cond_init(&qp->qready,NULL);
	if(err!=0) return err;
	err=pthread_mutex_init(&qp->cond_lock,NULL);
	if(err!=0) return err;
	qp->q_head=NULL;
	qp->q_tail=NULL;
	err=pthread_rwlock_init(&qp->q_lock,NULL);
	if(err!=0) return err;

	return 0;
}

/*
 * Insert a job at the head of the queue
 */
void job_insert(struct queue *qp,struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next=qp->q_head;//change the queue(the "next")list
	jp->j_prev=NULL;
	if(qp->q_head!=NULL) 
		qp->q_head->j_prev=jp;//change the queue(the "prev")list
	else
		qp->q_tail=jp;
	qp->q_head=jp;//Insert a job at the head of the queue
	pthread_rwlock_unlock(&qp->q_lock);
	pthread_cond_broadcast(&qp->qready);//condition is ready

}


void job_remove(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp)
            qp->q_tail = NULL;
        else
            jp->j_next->j_prev = jp->j_prev;
    } else if (jp == qp->q_tail) {
        qp->q_tail = jp->j_prev;
        jp->j_prev->j_next = jp->j_next;
    } else {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

struct job *job_find(struct queue *qp,pthread_t id)
{
	struct job *jp;
	if(pthread_rwlock_rdlock(&qp->q_lock)!=0)
		return (NULL);
	for(jp=qp->q_head;jp!=NULL;jp=jp->j_next)
		if(pthread_equal(jp->j_id,id))
			break;

	pthread_rwlock_unlock(&qp->q_lock);
	return jp;

}

void *worker_thread(void *arg)
{
	struct queue *qp=(struct queue*)arg;
	struct job *jp;
	
	//wait on condition..
	pthread_mutex_lock(&qp->cond_lock);
	while(true)
	{
		pthread_cond_wait(&qp->qready,&qp->cond_lock);

		printf("Thread %lu: Was waken up..",(unsigned long)pthread_self());
		jp=job_find(qp,pthread_self());

		if(jp){
			printf("   Got a job, doing stuff..");
			job_remove(qp,jp);
			free(jp);
		}
		printf("\n");
	}

}

#define NTHREADS 3

int main() {
    int err;
    pthread_t threads[NTHREADS];
    pthread_t next_worker_tid;
    struct queue queue;
    struct job *jp;

    srandom(time(NULL));

    // Init queue.
    err = queue_init(&queue);
    if (err != 0)
        err_exit(err, "can't init queue");

    // Launch a bunch of threads.
    for (int i = 0; i < NTHREADS; ++i) {
        err = pthread_create(&threads[i], NULL, worker_thread, &queue);
        if (err != 0)
            err_exit(err, "can't create thread %d", i);
    }

    // Add a few jobs.
    while (true) {
        jp =(struct job*) malloc(sizeof(struct job));
        next_worker_tid = threads[random() % NTHREADS];
        jp->j_id = next_worker_tid;
        printf("Master: queueing job for %lu\n", (unsigned long)next_worker_tid);
        job_insert(&queue, jp);
        sleep(1);
    }
}
