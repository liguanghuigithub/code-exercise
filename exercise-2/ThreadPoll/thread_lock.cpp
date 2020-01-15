#include "iostream"
#include "thread_lock.h"

using namespace std;

void ThreadLock::lock() const
{
    mtx.lock();
    //cout << "ThreadLock::lock()" << endl;
}

void ThreadLock::unlock() const
{
    mtx.unlock();
    //cout << "ThreadLock::unlock()" << endl;
}

