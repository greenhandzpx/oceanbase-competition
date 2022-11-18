#ifndef _MY_THREAD_POOL_H_
#define _MY_THREAD_POOL_H_

#include "lib/thread/ob_simple_thread_pool.h"


namespace oceanbase
{
namespace sql
{

class MyThreadPool: public common::ObSimpleThreadPool {
public:
    void run1() override 
    {
        ObTenantStatEstGuard state_est_guard(MTL_ID());
        ObTenantBase *tenant_base = MTL_CTX();
        Worker::CompatMode mode = ((ObTenant *)tenant_base)->get_compat_mode();
        Worker::set_compatibility_mode(mode);

        // common::ObSimpleThreadPool::run1();
        func_();
    }
    void handle(void *task) override 
    {
        if (task == nullptr) {
            // handle stop
            return;
        }
        (*(void(*)(void*))task)(args_);
    }

    void set_args(void *args)
    {
        args_ = args;
    }

    void set_func(const std::function<void()> &func) 
    {
        func_ = func;
    }
private:
    void *args_; // the global args used by all threads
    std::function<void()> func_;
};

}
}


#endif