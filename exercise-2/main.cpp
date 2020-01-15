// Example program
#include <iostream>
#include <fstream>
#include<signal.h>
#include<unistd.h>
#include <time.h>
#include <sys/time.h>
using namespace std;


void handler_signal(int n)
{
    if( SIGINT == n)
    {
        cout << "recv ctrl+c, do nothing" << endl;
        sleep(3);
    }
    else if(SIGQUIT == n)
    {
        cout << "recv ctrl+\\, do nothing" << endl;
    }
    else
    {
        cout << "recv unknown n:" << n << ", do nothing" << endl;
    }
}


class CUseTime{
public:
	CUseTime(std::string block_name):m_block_name(block_name){		
		gettimeofday(&m_stv, NULL);
		cout << "start block: " << m_block_name << ", " << m_stv.tv_sec << ", " << m_stv.tv_usec << endl;
	}
	~CUseTime(){				
		double used_time = getUsedTime(m_stv);
		cout << "end block: " << m_block_name <<", used " << used_time << " ms" << endl;
	}
	static double getUsedTime(struct timeval &stv){
		struct timeval etv;
		gettimeofday(&etv, NULL);
		cout << etv.tv_sec << ", " << etv.tv_usec << endl;
		double used_time = (etv.tv_sec - stv.tv_sec)*1000 + 0.001 * (etv.tv_usec - stv.tv_usec);
		return used_time;
	}
private:
	std::string m_block_name;
	struct timeval m_stv;
};
#include <stdlib.h>
int main(int argc, char** argv)
{
    //CUseTime use_tm("===============");
    //usleep(100);
    ofstream out("data.txt");
    srand(time(NULL));
    for(int i = 0; i != atoi(argv[1]); ++i)
    {
        out << random() % 10000 << endl;
    }
    out.close();
}

int main02()
{
    // SIG_DFL 正常终止
    // SIG_IGN 忽略
    signal(SIGINT, handler_signal); // SIGINT 一般是 CTRL+C
    // signal(SIGINT, SIG_IGN); // SIGINT 一般是 CTRL+C
    signal(SIGQUIT, handler_signal); 
    for(;;)
    {
        cout << "signal(SIGINT, SIG_IGN)" << endl;
        sleep(1);
    }
}
