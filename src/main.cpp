#include <cstdio>
#include "log/log.h"
#include <iostream>

int main(int argc, char **argv) {
  LOG_INFO("Mini LSM started with %d arguments", argc);
  
  std::cout << "Hello, Mini LSM!\n";

  LOG_INFO("Mini LSM exited successfully");
}
