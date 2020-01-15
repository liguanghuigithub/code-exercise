#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unistd.h>

using namespace std;

int a = 0;

mutex mtx;
std::condition_variable cv;

const int max_num = 101;

void f1()
{
    while(a < max_num)
    {
        unique_lock<mutex> lock(mtx);
        if(a&0x01)
        {
            cout << ++a << endl; 
            cv.notify_one();
        }
        if (a < max_num) // 此处很重要。不然打完最后一个数字后，可能会一直处于wait状态，但是另一个线程已经退出了
        {
            cv.wait(lock);
        }
    }
    cout << "f1 exit..." << endl;
}

void f2()
{
    while(a < max_num)
    {
        unique_lock<mutex> lock(mtx);
        if(!(a&0x01))
        {
            cout << ++a << endl; 
            cv.notify_one();
        }
        if (a < max_num) // 此处很重要。不然打完最后一个数字后，可能会一直处于wait状态，但是另一个线程已经退出了
        {
            cv.wait(lock);
        }
    }
    cout << "f2 exit..." << endl;
}

int main()
{
    thread t1(f1);
    thread t2(f2);

    t1.join();
    t2.join();
    cout << "main exit...." << endl;
}
