/*
 *基本使用
 */
#include"sync_log.h"

int main()
{
    
    LOGINIT("app_log/log","demo1.log");
    for(int i=0;i<1e2;++i)
    { 
        ERROR(<<"error "<<i<<endl);
        INFO(<<"info"<<endl);
        DEBUG(<<"debug"<<endl);
    }
}
