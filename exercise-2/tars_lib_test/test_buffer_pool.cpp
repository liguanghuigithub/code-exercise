#include "util/tc_buffer_pool.h"
#include <iostream>
#include <string.h>

using namespace std;
using namespace tars;

int main()
{
    TC_BufferPool* pool  = new TC_BufferPool(30, 2000);
    char p[] = "1234567890";
    TC_Slice slice = pool->Allocate(sizeof(p));
    cout << slice.data << " " << slice.len << endl;
    memcpy(slice.data, p, sizeof(p));
    char * data = (char *)slice.data;
    cout << data << endl;
    while(data && *data)
    {
        cout << *data << " ";
        data++;
    }
    cout << endl;
    delete pool;
    return 0;
}
// g++ test_buffer_pool.cpp /home/lgh/tars_code/cpp/util/src/tc_buffer_pool.cpp -I/home/lgh/tars_code/cpp/util/include && ./a.out
