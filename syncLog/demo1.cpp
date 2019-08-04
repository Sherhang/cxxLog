/*
 *基本使用
 */
#include"sync_log.h"

int main()
{
    
    LOGINIT("app_log/log","demo1.log");
    for(int i=0;i<1e5;++i)
    { 
        ERROR(<<"error"<<endl);
        INFO(<<"info"<<endl);
        DEBUG(<<"debug"<<endl);
    }
}
