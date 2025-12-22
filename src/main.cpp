#include <cstdio>
#include "log/log.h"
#include <iostream>

int main(int argc, char **argv) {
  // init logger
  mini_lsm::Logger &logger = mini_lsm::Logger::get_instance();
  logger.set_log_file("mini_lsm.log", mini_lsm::Logger::DEFAULT_MAX_FILE_SIZE,
                      mini_lsm::Logger::DEFAULT_MAX_FILE_COUNT);
  logger.set_log_level(mini_lsm::LogLevel::DEBUG);

  LOG_INFO("\nMini LSM started with %d arguments\n", argc);

  std::cout << "Hello, Mini LSM!\n";

  LOG_INFO("\nMini LSM exited successfully\n");
}
