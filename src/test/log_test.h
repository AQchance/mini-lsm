#include "log/log.h"

namespace test {

void log_test() {
    using namespace mini_lsm;

    Logger::get_instance().set_log_level(LogLevel::DEBUG);
    Logger::get_instance().set_log_file("mini_lsm.log");
    LOG_INFO("\n=== example 1: basic log output ===\n");
    LOG_DEBUG("this is a DEBUG log");
    LOG_INFO("this is an INFO log");
    LOG_WARN("this is a WARN log");
    LOG_ERROR("this is an ERROR log");

    LOG_INFO("\n=== emample 2: use it like printf ===\n");
    int value = 42;
    const char *name = "mini-lsm";
    LOG_INFO("project name: %s, value: %d", name, value);
    LOG_DEBUG("hex: 0x%x, float: %.2f", 255, 3.14159);

    LOG_INFO("\n=== example 3: set log level WARN ===\n");
    Logger::get_instance().set_log_level(LogLevel::WARN);
    LOG_DEBUG("this DEBUG log will not output");
    LOG_INFO("this INFO log will not output");
    LOG_WARN("this WARN log will output");
    LOG_ERROR("this ERROR log will output");

    Logger::get_instance().set_log_level(LogLevel::DEBUG);
    LOG_INFO("\n=== log test end! ===\n");
}

} // namespace test
