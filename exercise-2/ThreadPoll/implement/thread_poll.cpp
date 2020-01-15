#include "thread_poll.h"

ThreadPoll::ThreadPoll(int _thread_num) : thread_num(_thread_num)
{
    for(int i = 0; i <= thread_num; ++i)
    {
        threads.push_back([](){}
        );
    }

    task_list.resize(thread_num);
}

ThreadPoll::~ThreadPoll()
{
    cv.notify_all();
    for(auto& td : threads)
    {
        if(td.joinable())
        {
            td.join();
        }
    }
}

ThreadPoll::addTask(std::function<void()> task)
{
}
