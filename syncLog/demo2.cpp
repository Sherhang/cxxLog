/*多线程版本测试
 * 
 */
#include <iostream>
#include <thread>
#include "sync_log.h"
using namespace std;

void display(int id)
{
    for(int i=0;i<1e2;++i)
    {
        DEBUG(<<"This is a test! ID is "<<id<<endl);
    }
}

int main()
{
    LOGINIT("app_log","demo2");
    thread th[10];
    for(int i=0;i<10;++i)
    {
        th[i] = thread(display,i);
    }
    for(int i=0;i<10;++i)
    {
        th[i].join();
    }
}
