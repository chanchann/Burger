#include "burger/base/coroutine.h"
#include "burger/base/Log.h"
#include <vector>
#include <thread>
#include <algorithm>

using namespace burger;

void runInCo() {
    INFO("RUN IN coroutine begin");
    Coroutine::YieldToHold();
    INFO("RUN IN coroutine end");
    Coroutine::YieldToHold();
}


void testCo() {
    INFO("main begin -1");
    {
        Coroutine::GetThis();  
        INFO("main begin");
        Coroutine::ptr co = std::make_shared<Coroutine>(runInCo);
        co->swapIn();
        INFO("main after swapIn");
        co->swapIn();
        INFO("main after end");
        co->swapIn();
    }
    INFO("main after end2");
}

void print() {
    std::cout << "123" << std::endl;
}
int main() {
    LOGGER(); LOG_LEVEL_DEBUG;
    std::vector<std::thread> threads;
    for(int i = 0; i < 3; i++) {
        threads.push_back(std::thread(testCo));
        // threads.push_back(std::thread(print));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return 0;
    
}