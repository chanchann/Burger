#include "burger/net/Scheduler.h"
#include "burger/base/Log.h"
#include <chrono>
#include <thread>
using namespace burger;
using namespace burger::net;

// todo : 未完成

void foo() {
    INFO("IN foo");
}

int main() {
    LOGGER(); LOG_LEVEL_TRACE;
    Scheduler sched;

    sched.startAsync();
    sched.addTask(foo, "foo");
    std::cout << "prior to foo" << std::endl;
    sched.wait();
    return 0;
}