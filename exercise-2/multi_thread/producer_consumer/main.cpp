#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;


template<typename T>
ostream& operator << (ostream& os, vector<T> vec)
{
    for(const auto& t : vec)
    {
        os << t << " ";
    }
    return os;
}

vector<int> vec;
std::mutex mtx;
std::condition_variable cv;
const int interval = 200; // milliseconds

void producer()
{
    int i = 0;
    while(false || i < 100)
    {
        {
        unique_lock<std::mutex> lock(mtx);
        while(vec.size() >= 10)
        {
            cout << "vec.size():" << vec.size() << endl;
            // wait()
            cout << "======== producer wait =========== " << endl;
            cv.wait(lock);
            cout << "======== producer receive notify =========== " << endl;
        }
        vec.push_back(i++);
        cout << "producer.  vec:\t" << vec << endl;
        if(1 == vec.size())
        {
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~ vec.size() == 1, start notify consumer" << endl;
            cv.notify_one();
        }
        // signal consumer
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2 * interval));
    }

}

void consumer()
{
    while(true)
    {
        {
        unique_lock<std::mutex> lock(mtx);
        if(!vec.empty())
        {
            vec.erase(vec.end() - 1);
            cout << "consumer.  vec:\t" << vec << endl;
            if(vec.size() == 5)
            {
                // signal producer
                cout << "======== notify producer =========== " << endl;
                cv.notify_one();
            }
        }
        else
        {
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~ vec is empty. wait..." << endl;
            cv.wait(lock);
        }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(int(2 * interval)));
    }
}

int main()
{
    thread t1(producer);
    thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}

void f1(int & n)
{
    lock_guard<std::mutex> lock(mtx);
    for(int i = 0; i != 50000000; ++i)
    {
        ++n;
    }
}

void f2(int & n)
{
    lock_guard<std::mutex> lock(mtx);
    for(int i = 0; i != 50000000; ++i)
    {
        --n;
    }
}

int main02()
{
    int n = 0;
    thread t1(f1, std::ref(n)), t2(f2, std::ref(n));
    t1.join();
    t2.join();
    cout << "n: " << n << endl;
    return 0;
}
