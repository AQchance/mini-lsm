#include "log/log.h"

namespace test {

void log_test() {
    using namespace mini_lsm;

    Logger::get_instance().set_log_level(LogLevel::DEBUG);
    Logger::get_instance().set_log_file("mini_lsm.log");
    LOG_INFO("=== 示例1: 基本日志输出 ===\n");
    LOG_DEBUG("这是一条 DEBUG 日志");
    LOG_INFO("这是一条 INFO 日志");
    LOG_WARN("这是一条 WARN 日志");
    LOG_ERROR("这是一条 ERROR 日志");

    LOG_INFO("\n=== 示例2: printf 风格格式化 ===\n");
    int value = 42;
    const char *name = "mini-lsm";
    LOG_INFO("项目名称: %s, 值: %d", name, value);
    LOG_DEBUG("十六进制: 0x%x, 浮点数: %.2f", 255, 3.14159);

    LOG_INFO("\n=== 示例3: 设置日志级别为 WARN ===\n");
    Logger::get_instance().set_log_level(LogLevel::WARN);
    LOG_DEBUG("这条 DEBUG 不会输出");
    LOG_INFO("这条 INFO 不会输出");
    LOG_WARN("这条 WARN 会输出");
    LOG_ERROR("这条 ERROR 会输出");

    LOG_INFO("\n=== 日志系统测试完成 ===\n");
}

} // namespace test
