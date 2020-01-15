#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <functional>
#include <unistd.h>
#include <thread>
#include <sstream>

using namespace std;


#include "tc_thread.h"
#include "tc_thread_rwlock.h"
#include "tc_thread_mutex.h"
#include "tc_lock.h"


using namespace tars;

TC_ThreadRWLocker _rw_mtx;
TC_ThreadMutex mtx;
TC_ThreadRecMutex _rec_mtx;


int main()
{
    TC_LockT<TC_ThreadRecMutex> _lock(_rec_mtx);
    TC_LockT<TC_ThreadRecMutex> _lock1(_rec_mtx);
    TC_LockT<TC_ThreadRecMutex> _lock2(_rec_mtx);
    TC_LockT<TC_ThreadRecMutex> _lock3(_rec_mtx);
    cout << "main thread:" << this_thread::get_id() << endl;
    //cout << "finish.................. ^_^" << endl;
    return 0;
}

// g++ --std=c++11 -pthread thread_test.cpp -I/usr/local/tars/cpp/include/util/ -L/usr/local/tars/cpp/lib/ -ltarsutil -I/usr/local/tars/cpp/include/ && ./a.out
