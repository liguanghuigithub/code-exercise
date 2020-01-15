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
#include "tc_atomic.h"


using namespace tars;

class MyThread : public TC_Thread
{
public:
    static TC_Atomic cnt;
    MyThread()
    {
        ++cnt;
    }

    void run()
    {
        ostringstream os;
        os << "run thread id:" << getThreadControl().id();
        cout << os.str() << endl;
        //cout << "run thread id:" << getThreadControl().id() << endl;
        while(cnt < 100)
        {
            //cout << "thread id:" << (int)getThreadControl().id() << ", cnt:" << cnt << endl;
            cout << " cnt:" << ++cnt << endl;
        }
    }
};

TC_Atomic MyThread::cnt = 0;

int main()
{
    MyThread* t1 = new MyThread;

    TC_ThreadControl control = t1->start();
    control.join();
    sleep(0.1); 
    cout << "main thread:" << this_thread::get_id() << endl;
    //cout << "finish.................. ^_^" << endl;
    return 0;
}

// g++ --std=c++11 -pthread thread_test.cpp -I/usr/local/tars/cpp/include/util/ -L/usr/local/tars/cpp/lib/ -ltarsutil -I/usr/local/tars/cpp/include/ && ./a.out
