/* This file is part of the atomic lab and is derived based on the code from ARM's website at:
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dht0008a/ch01s03s02.html .
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 * License: Creative Commons: Attribution, Share-Alike license
 * For details please refer to http://creativecommons.org/licenses/by-sa/3.0/.
 */

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define locked 1
#define unlocked 0

extern void lock_mutex(void *mutex);
extern void unlock_mutex(void *mutex);

pthread_t tid[2];
int counter;
//pthread_mutex_t lock;
unsigned int mutexlock = unlocked;

void* doSomeThing(void *arg)
{
   //pthread_mutex_lock(&lock);
    lock_mutex(&mutexlock); 
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d started\n", counter);

    for(i=0; i<(0xFFFFFFFF);i++);

    printf("\n Job %d finished\n", counter);

    //pthread_mutex_unlock(&lock);
    unlock_mutex(&mutexlock);
    return NULL;
}

int main(void)
{
    int i = 0;
    int err;

    //if (pthread_mutex_init(&lock, NULL) != 0)
    //{
    //    printf("\n mutex init failed\n");
    //    return 1;
    //}

    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    //pthread_mutex_destroy(&lock);
    
    return 0;
}
