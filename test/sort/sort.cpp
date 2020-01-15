#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;

template<typename T>
ostream& operator << (ostream& os, const vector<T>& vec)
{
    for(const auto& val : vec)
    {   
        os << val << " ";
    }   
    return os; 
}

template<typename Function, typename T, typename ... Args>
void test(const Function& f, vector<T>& vec, Args... args)
{
    random_shuffle(vec.begin(), vec.end());
    cout << "random_shuffle:" << vec << endl;
    f(vec, args...);
    cout << vec << endl;
}

// 冒泡
void bubbleSort(vector<int>& vec)
{
    cout << __FUNCTION__ << ": ";
    
    for(int i = vec.size()-1; i >=0 ; --i)
    {   
        for(int j = 0; j != i; ++j)
        {   
            if (vec[j] > vec[j+1])
            {   
                std::swap(vec[j], vec[j+1]);
            }   
        }   
    }   
}

// 交换排序
void swapSort(vector<int>& vec)
{
    cout << __FUNCTION__ << ": ";
    for(int i = vec.size()-1;i >=0;--i)
    {
        for(int j=0;j<i;++j)
        {
            if (vec[i] < vec[j])
            {
                std::swap(vec[i], vec[j]);
            }
        }
    }
}

// 快排
// 前闭后开

int partition(vector<int>& vec, int b, int e)
{
    int i = b-1, j=b;
    int key = vec[e];
    for(int j = b; j != e; ++j)
    {
        if (vec[j] < key)
        {
            ++i;
            if (i != j)
            {
                std::swap(vec[i], vec[j]);
            }
        }
    }
    std::swap(vec[++i], vec[e]);
    return i;
}

void quickSort(vector<int>& vec, int b, int e)
{
    if (b < e)
    {
        int i = partition(vec, b, e);
        quickSort(vec, b, i-1);
        quickSort(vec, i+1, e);
    }
}

void insertSort(vector<int>& vec)
{
    if (vec.size() < 1)
    {
        return;
    }
    int j = 1;
    while(j < vec.size())
    {
        int i = j;
        while(i > 0 && vec[i] < vec[i-1])
        {
            std::swap(vec[i], vec[i-1]);
            i--;
        }
        j++;
    }
}

int main()
{
    vector<int> vec{8,7,1,9,4,4,0};
    test(bubbleSort, vec);
    test(swapSort, vec);
    test(quickSort, vec, 0, vec.size()-1);
    test(insertSort, vec);

    return 0;
}
