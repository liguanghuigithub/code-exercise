#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;

using cal = std::function<int(int, int)>;

map<char, cal> calMap
{
{'+', [](int a, int b){return a+b;}}
};

int main()
{
    int a, b;
    cin >> a >> b;
    char method;
    cin >> method;

    cout << "a:" << a << ", b:" << b << ", method:" << method << endl;

    auto it = calMap.find(method);
    if (calMap.end() == it)
    {
        cout << "not find method:" << method << endl;
        return 1;
    }

    cout << it->second(a, b) << endl;

    return 0;
}
