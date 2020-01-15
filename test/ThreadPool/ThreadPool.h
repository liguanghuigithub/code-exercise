#ifndef _TREAD_POOL_H_
#define _TREAD_POOL_H_
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <list>
#include <unistd.h>

using namespace std;

atomic<int> cnt;

class ThreadTask
{
    //virtual void exec() = 0;
public:
    void exec()
    {
        cout << "thread id:" << std::this_thread::get_id() << endl;
        cnt++;
    }

};
using ThreadTaskPtr = ThreadTask*;

class ThreadWorker
{
public:
    ThreadWorker();
    void start();
    bool addTask(ThreadTaskPtr pTask);
    void terminate();
private:
    thread t;
    bool terminated;
    mutex mtx;
    condition_variable cv;
    list<ThreadTaskPtr>  _task_list;
};
using ThreadWorkerPtr = ThreadWorker*;

class ThreadPool
{
public:
    ThreadPool(int thread_num);
    ~ThreadPool();
    bool addTask(ThreadTaskPtr pTask);

    void terminate();
    
private:
    bool terminated;
    int _thread_num;
    vector<ThreadWorkerPtr> _thread_work_list;
};

#endif
