/*多线程版本测试
 * 
 */
#include <iostream>
#include <thread>
#include "sync_log.h"
#include <unistd.h>
using namespace std;

void display(int id)
{
    uint64_t i =0;
    while(1)

    {
        sleep(1);
        i++;
        DEBUG(<<"This is a test! ID is "<<id<<endl);
        //ERROR(<<"This is a test! ID is "<<id<<endl);
    }
}

int main()
{
    int n =1;
    LOGINIT("app_log","demo2");
    thread th[n];
    for(int i=0;i<n;++i)
    {
        th[i] = thread(display,i);
    }
    for(int i=0;i<n;++i)
    {
        th[i].join();
    }
}
