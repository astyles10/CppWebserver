#include <iostream>

constexpr auto gcd(int a, int b) {
  while (b != 0) {
    auto t = b;
    b = a % b;
    a = t;
  }
  return a;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::printf("Usage: %s <number1> <number2>\n", argv[0]);
    return;
  }

  constexpr uint32_t aArg1Value = 11;
  constexpr uint32_t aArg2Value = 111;

  constexpr int i = gcd(aArg1Value, aArg2Value);

  int a = 11;
  int b = 121;
  int j = gcd(a, b);

  std::cout << "gcd(11,121): " << i << std::endl;
  std::cout << "gcd(a,b): " << j << std::endl;

  std::cout << std::endl;
}