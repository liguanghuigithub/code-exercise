#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

pthread_key_t key;
pthread_key_t key2;
pthread_t thid1;
pthread_t thid2;

void* thread2(void* arg)
{
    printf("thread:%lu is running\n", pthread_self());
    
    int key_va = 3 ;

    pthread_setspecific(key, (void*)key_va);
    sleep(2);
    
    printf("thread:%lu return %p\n", pthread_self(), pthread_getspecific(key));
}


void* thread1(void* arg)
{
    printf("thread:%lu is running\n", pthread_self());
    
    int key_va = 5;
    
    //pthread_setspecific(key, (void*)key_va);

    pthread_create(&thid2, NULL, thread2, NULL);

    printf("thread:%lu return %p\n", pthread_self(), pthread_getspecific(key));
}

#pragma pack(2)
struct A
{
    long b;
    int a;
    bool c;
};


int main()
{
    A* pA = new A[100];

    cout << "pA:" << pA << ", size:" << sizeof(pA) << endl;

    int * pI = NULL;
    cout << sizeof(pI) << endl;

    cout << sizeof(int) << ", " << sizeof(long) << ", " << sizeof(A*) << ", " << sizeof(int*) << endl;

    A pB[100];

    cout << "pB:" << pB << ", size:" << sizeof(pB) << endl;

    
    printf("main thread:%lu is running\n", pthread_self());

    pthread_key_create(&key, NULL);

    printf("key:%d\n", &key);

    pthread_key_create(&key2, NULL);
    printf("key2:%d\n", &key2);

    pthread_create(&thid1, NULL, thread1, NULL);

    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);

    int key_va = 1;
    pthread_setspecific(key, (void*)key_va);
    
    printf("thread:%lu return %p\n", pthread_self(), pthread_getspecific(key));

    pthread_key_delete(key);
        
    printf("main thread exit\n");
    return 0;
}
