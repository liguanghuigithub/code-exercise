#include <iostream>
#include <vector>
#include <mutex>
#include <numeric>
#include <functional>
#include <algorithm>
#include <thread>
#include "thread_lock.h"
#include <unistd.h>
#include <chrono>
#include <ratio>

using namespace std;
using namespace std::chrono;

class CUseTime
{

public:
    CUseTime(string _block) : block(_block), start_point(high_resolution_clock::now())
    {
        cout << "start block:" << block << endl;//", start time:" << start_point << endl;
    }

    ~CUseTime()
    {
        cout << "end block:" << block << ", cost time:" << duration_cast<duration<double>>(high_resolution_clock::now() - start_point).count() << endl;
    }

private:
    string block;
    high_resolution_clock::time_point start_point;
    
};


template<typename T>
ostream& operator << (ostream& os, const vector<T> vec)
{
    for_each(vec.begin(), vec.end(), [&](const T t){os << t << ends;});
}


void f(int n)
{
    CUseTime use_tm("sleep " + std::to_string(n));
    std::this_thread::sleep_for(std::chrono::seconds(n));
}

int main()
{

    ThreadLock thread_lock;
    vector<long> vec;
    long sum = 0;
    int num = 1;

    int b = std::move(num);
    cout << "num:" << num << ", &num:" << &num << endl;
    cout << "b:" << b << ", &b:" << &b << endl;

    string c("cccccccccc");
    cout << "&c:" << &c << endl;
    //string d = std::move(c);
    string d = c;
    cout << "c:" << c << endl;
    cout << "d:" << d << endl;

    cout << "&c:" << &c << endl;
    cout << "&d:" << &d << endl;
    
    //thread t1([&](){for(int i=1; i<= num;++i){ThreadAutoLock<ThreadLock> lock2(thread_lock); vec.push_back(i); sum+=i;}});
    //thread t2([&](){for(int i=1; i<= num;++i){ThreadAutoLock<ThreadLock> lock2(thread_lock); vec.push_back(i); sum+=i;}});
    CUseTime use_tm("main");

    thread t3(f, 3);
    thread t2(f, 2);
    thread t4(f, 6);

   {CUseTime u1("--------------");
   t4.join();}

   {CUseTime u1("~~~~~~~~~~~~~~");
   t3.join();}

   {CUseTime u1("!!!!!!!");
   t2.join();}
   //std::this_thread::sleep_for(std::chrono::seconds(7));
}
