// Example program
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>

using namespace std;

struct A
{
int groupId;
int msgId;

bool operator == (const A& a) const
{
    return (groupId == a.groupId && msgId == a.msgId);
}

bool operator < (const A& x) const
{
    return groupId < x.groupId || msgId < x.msgId;
    return groupId < x.groupId || (groupId == x.groupId && msgId < x.msgId);
}

};

int main()
{
  std::string name;
  A a{1,2};
  A b{1,3};
  A c{2,1};
  cout << (a < b) << endl;
  cout << (b < c) << endl;
  cout << (c < b) << endl;
  cout << (c < a) << endl;
  set<A> myset;
  myset.insert(c);
  myset.insert(b);
  myset.insert(a);
  for(auto& data : myset)
  {
      cout << data.groupId << ".." << data.msgId << endl;
  }
}

