#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;

struct ObServer
{
protected:
    string name;
public:
    ObServer(const string& _name) : name(_name)
    {
    }

    virtual void update(const string& msg)
    {
        cout << typeid(*this).name()  << "::" << __FUNCTION__ << ". name:" << name << ", msg:" << msg << endl;
    }
};

class ConcreteObServer : public ObServer
{
public:
    ConcreteObServer(const string& _name) : ObServer(_name)
    {
    }

    virtual void update(const string& msg)
    {
        cout << typeid(*this).name()  << "::" << __FUNCTION__ << ". name:" << name << ", msg:" << msg << endl;
    }
};

typedef ObServer* ObServerPtr;

struct Subject
{
private:
    set<ObServer*> ObServerPtrSet;
public:
    bool attach(ObServerPtr pObServer)
    {
        if(!pObServer)
        {
            return false;
        }
        ObServerPtrSet.insert(pObServer);
        return true;
    }

    void detach(ObServerPtr pObServer)
    {
        ObServerPtrSet.erase(pObServer);
    }

    void notify()
    {
        for_each(ObServerPtrSet.begin(), ObServerPtrSet.end(), [](ObServerPtr pObServer){pObServer->update("hello");});
    }
};

using namespace std;

int main()
{
    Subject subject;
    subject.attach(new ObServer("A"));
    subject.attach(new ObServer("B"));

    Subject subject2;
    subject2.attach(new ObServer("A"));
    subject2.attach(new ConcreteObServer("C"));

    subject.notify();
    subject2.notify();
    cout << "finish.................." << endl;
    return 0;
}
