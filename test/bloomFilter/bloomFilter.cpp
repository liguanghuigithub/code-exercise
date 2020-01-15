// Example program
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>

using namespace std;

class Filter
{
private:
     using EnumType = unsigned char;
     static const int _perSize = sizeof(EnumType) * 8;
private:
     vector<EnumType> buffer;
     int _len;

private:
    struct Pos
    {
        long index;
        int offset;
    };

    Pos getPos(const unsigned long& val)
    {
        Pos ret;
        ret.index = val / _perSize;
        ret.offset = val % _perSize;
        cout << "index:" << ret.index << ", offset:" << ret.offset << endl;
        return ret;
    }

public:
    Filter(int len);
    ~Filter();

    void add(const unsigned long& val);

    bool exists(const unsigned long& val);
};

Filter::Filter(int len) : _len(len)
{
    buffer.resize(_len);
    cout << "Filter::Filter" << endl;
}

Filter::~Filter()
{
    cout << "Filter::~Filter()" << endl;
}

void Filter::add(const unsigned long& val)
{
    Pos pos = getPos(val);
    buffer[pos.index] |= 0x1 << pos.offset;
}

bool Filter::exists(const unsigned long& val)
{
    Pos pos = getPos(val);
    return (buffer[pos.index] &= 0x1 << pos.offset);
}

int main()
{
    Filter filter(1000 * 1000);
    filter.add(10000);
    cout << filter.exists(10000) << endl;
    cout << filter.exists(1000) << endl;
}
