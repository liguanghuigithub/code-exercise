#include "ThreadPool.h"

ThreadWorker::ThreadWorker() : terminated(false)
{
}

bool ThreadWorker::addTask(ThreadTaskPtr pTask)
{
    if (terminated)
    {
        return false;
    }

    if (!pTask)
    {
        return false;
    }

    unique_lock<mutex> lock(mtx);
    _task_list.push_back(pTask);
    cv.notify_one();
    return true;
}

void ThreadWorker::start()
{
    t = thread([&]() {
        while (true)
        {
            ThreadTaskPtr pTask = NULL;
            {
                unique_lock<mutex> lock(mtx);
                if (_task_list.empty())
                {
                    if (!terminated)
                    {
                        cv.wait(lock);
                    }
                    else
                    {
                        break;
                    }
                }

                if(!_task_list.empty())
                {
                    pTask = _task_list.front();
                    _task_list.pop_front();
                }
            }
            if (pTask)
            {
                pTask->exec();
                if (pTask)
                {
                    delete pTask;
                }
            }
        }
    });
}

void ThreadWorker::terminate()
{
    terminated = true;
    cv.notify_one();
    if (t.joinable())
    {
        t.join();
    }
}

ThreadPool::ThreadPool(int thread_num) : terminated(false), _thread_num(thread_num)
{
    if (_thread_num <= 0)
    {
        terminated = true;
    }

    for(int i = 0; i != _thread_num; ++i)
    {
        _thread_work_list.push_back(new ThreadWorker);
        _thread_work_list[i]->start();
    }
}

ThreadPool::~ThreadPool()
{
    terminate();
}

bool ThreadPool::addTask(ThreadTaskPtr pTask)
{
    if (terminated)
    {
        return false;
    }

    if (!pTask)
    {
        return false;
    }

    int idx = rand() % _thread_num;
    return _thread_work_list[idx]->addTask(pTask);
}

void ThreadPool::terminate()
{
    for(auto& pThreadWorker : _thread_work_list)
    {
        pThreadWorker->terminate();
    }
}
