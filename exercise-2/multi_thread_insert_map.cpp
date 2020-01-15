#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>

using namespace std;

void test_insert(map<int, string>& dest, int i)
{
    if(i <= 0)
    {   
        return;
    }   
    for(int n = 0; n != i*i; ++n)
    {   
        dest[n] = std::to_string(rand() % 100) + std::to_string(i + 10);
    }   
}

template<typename U, typename V>
ostream& operator << (ostream& os, map<U, V> mymap)
{
    for(auto it = mymap.begin(); it != mymap.end(); ++it)
    {
        os << "[ " << it->first << " ---> " << it->second << " ] "; 
    }
    return os;
}

int main()
{
    vector<thread> threads;
    map<int, string> mymap;
    for(int i = 1; i <= 100; ++i)
    {
        threads.push_back(thread(test_insert, std::ref(mymap), i));
    }   

    for(int i = 0; i != static_cast<int>(threads.size()); ++i)
    {   
        threads[i].join();
    }
    cout << mymap << endl;
    cout << mymap.size() << endl; 
    return 0;
}
