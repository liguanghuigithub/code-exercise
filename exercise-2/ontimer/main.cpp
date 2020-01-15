// Example program
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>

#include "util/tc_thread.h"
#include "util/tc_timer_manager.h"
#include "util/tc_autoptr.h"

using namespace std;
using namespace tars;

#define TLOGINFO(s) cout << s

void test()
{
    int i = 0;
    while(true)
    {
        cout << "hello...." << endl;
        sleep(1);
    }
}

void test1()
{
    cout << "hello, test1..." << time(NULL) << endl;
    sleep(2);
}

class EndPointTimer : public TC_Thread, public TC_HandleBase
{
public:
    EndPointTimer(double _interval, TimerCallback _cb, bool _runRightNow = true) : interval(_interval), cb(_cb), runRightNow(_runRightNow){}
    
    virtual void run()
    {   
        TLOGINFO("check interval:" << interval << "runRightNow:" << runRightNow << endl);
        timer_manager.runEvery(interval, cb, runRightNow);
        timer_manager.run();
    }   

private:
    TC_TimerManager timer_manager;
    double interval;
    TimerCallback cb; 
    bool runRightNow;
};

int main()
{
    TC_AutoPtr<EndPointTimer> p_timer;
    p_timer = new EndPointTimer(3, test1, true);
    p_timer->start();
    p_timer->getThreadControl().detach();
    cout << "in main......." << endl;
    sleep(100);
    
}
