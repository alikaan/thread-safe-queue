#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


# define SIZE 100
void enqueue();
void dequeue();
void show();
int inp_arr[SIZE];
int Rear = - 1;
int Front = - 1;

pthread_mutex_t queue_lock;

int get_random()
{
    int lower = 1, upper = 100;
    return (rand() % (upper - lower + 1)) + lower;
}

void print_date()
{
    time_t t = time(NULL);
    struct tm* tm_local = localtime(&t); 
    printf("[%d:%d:%d] : ", tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec);
}

void print_sperator()
{
    for(int i = 0; i < 20; i++)
        printf("-");

    printf("\n");
}

void enqueue(int item)
{
    int insert_item;
    if (Rear == SIZE - 1)
    {
        print_date();
        printf("overflow \n");
    }
    else
    {
        if (Front == - 1)
      
        Front = 0;
        print_date();
        printf("item : %d\n", item);
        insert_item = item;
        Rear = Rear + 1;
        inp_arr[Rear] = insert_item;
    }
} 
 
void dequeue()
{
    if (Front == - 1 || Front > Rear)
    {
        print_date();
        printf("underflow \n");
        return ;
    }
    else
    {
        print_date();
        printf("item deleted from the Queue: %d\n", inp_arr[Front]);
        Front = Front + 1;
    }
} 
 
void show()
{
    if (Front == - 1)
    {
        print_date();
        printf("Empty Queue \n");
    }   
    else
    {
        print_date();
        printf("Queue: \n\t");
        for (int i = Front; i <= Rear; i++)
            printf("%d ", inp_arr[i]);
        printf("\n");
    }
}

void *enqueuer_thread(void *vargp)
{
    int *myid = (int *)vargp;
    sleep(1);
    for(;;)
    {
        pthread_mutex_lock(&queue_lock);
        print_sperator();
        print_date();
        printf("hello from %s, thread id: %d\n", __FUNCTION__, *myid);
        int val = get_random();
        enqueue(val);
        show();
        print_date();
        printf("bye from %s, thread id: %d\n", __FUNCTION__, *myid);
        print_sperator();
        pthread_mutex_unlock(&queue_lock);
        sleep(5);
    }
    return 0;
}

void *dequeuer_thread(void *vargp)
{
    int *myid = (int *)vargp;

    sleep(1);
    for(;;)
    {
        pthread_mutex_lock(&queue_lock);
        print_sperator();
        print_date();
        printf("hello from %s, thread_id: %d\n", __FUNCTION__, *myid);
        int val = get_random();
        dequeue();
        show();
        print_date();
        printf("bye from %s, thread_id: %d\n", __FUNCTION__, *myid);
        print_sperator();
        pthread_mutex_unlock(&queue_lock);
        sleep(5);
    }
    return 0;
}
  
int main()
{
    if (pthread_mutex_init(&queue_lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    
    pthread_t thread_id_1, thread_id_2, thread_id_3;
    printf("before creating threads\n");
    pthread_create(&thread_id_1, NULL, enqueuer_thread, (void *)&thread_id_1);
    pthread_create(&thread_id_2, NULL, dequeuer_thread, (void *)&thread_id_2);
    pthread_create(&thread_id_3, NULL, enqueuer_thread, (void *)&thread_id_3);

    printf("after creating threads\n");
    printf("press ctrl+c to exit\n");
    while(1);
}