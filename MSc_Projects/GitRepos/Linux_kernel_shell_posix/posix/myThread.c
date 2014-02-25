#include <stdio.h>
#include <stdlib.h>

void *myfunc (void *myvar);

int main(int argc, char *argv[])
{
	pthread_t thread1,thread2;
	char *mssg1 = "First thread";
	char *mssg2 = "second thread";
	int ret1, ret2;

	ret1 = pthread_create(&thread1,NULL,myfunc,(void *)mssg1);
	ret2 = pthread_create(&thread2,NULL,myfunc,(void *)mssg2);
	
	printf("main function after creation of thread \n");
	
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	printf("First thread return = %d \n",ret1);
	printf("Second thread return = %d \n", ret2);

	return 0;
}

void *myfunc(void *myvar)
{
	char *mssg;
	mssg = (char *) myvar;
	int i;
	for (i=0; i<10; i++)
 	{
	 printf("%s %d \n",mssg,i);
	 sleep(1);
	}
	return NULL;
}
