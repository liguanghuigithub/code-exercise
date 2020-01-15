#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <unistd.h>

using namespace std;

int a = 1;

void f1()
{
    a = 2;
    while(a==2)
    {
        cout << "f1.." << endl;
        sleep(1);
    }
    cout << "f1 out of while." << endl;
    sleep(10);
    a = 4;
    cout << "f1 exit..." << endl;
}

void f2()
{
    a = 3;
    while(a==3)
    {
        cout << "f2.." << endl;
        sleep(1);
    }

    cout << "f2 out of while." << endl;
    sleep(10);
    cout << "f2 exit..." << endl;
}

int main()
{
    thread t1(f1);
    sleep(5);
    thread t2(f2);

    t1.join();
    t2.join();

    cout << "main  exit..." << endl;
}
