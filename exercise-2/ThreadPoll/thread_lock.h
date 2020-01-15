#include <mutex>

using namespace std;


class ThreadLock
{
private:
    mutable mutex mtx;

public:
    void lock() const;
    void unlock() const;
};


template<typename T>
class ThreadAutoLock
{
public:
    ThreadAutoLock(const T& _mtx) : mtx(_mtx)
    {
        mtx.lock();
    }

    ~ThreadAutoLock()
    {
        mtx.unlock();
    }

protected:
    const T& mtx;
};
