#include "test/log_test.h"
#include <iostream>

// 下面可以用来运行测试
int main() {
  // 日志测试
  std::cout << "Running log tests...\n";
  test::log_test();
  std::cout << "Log tests completed.\n";
  return 0;
}
