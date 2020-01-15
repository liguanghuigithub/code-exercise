#include <iostream>
#include <string>
using namespace std;
#define INT_MIN -2147483648
int main()
{
      long long num=-321;
      if(num<INT_MIN)
        num=0;
      cout<<num<<endl;
}
