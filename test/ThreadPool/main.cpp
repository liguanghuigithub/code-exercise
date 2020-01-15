#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main()
{
    ThreadPool pool(10);
    for(int i = 0; i != 10; ++i)
    {
        pool.addTask(new ThreadTask);
    }
    pool.terminate();
    cout << "main  exit..." << endl;
}
