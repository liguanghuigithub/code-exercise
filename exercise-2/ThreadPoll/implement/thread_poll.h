#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class ThreadWorker
{
    
};


class ThreadPoll
{
private:
    condition_variable  cv;
    mutex mtx;

private:
    int thread_num;
    vector<thread> threads;
    vector<list<std::function<void()> > > tasks_list;

public:
    ThreadPoll(int _thread_num = 20);
    ~ThreadPoll();
    void addTask(std::function<void()> task);
};
