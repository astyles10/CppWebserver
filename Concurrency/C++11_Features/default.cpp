#include <iostream>

class MyDefault {
 public:
 // Defaulted functions are written by the compiler
  MyDefault() = default;
  ~MyDefault() = default;

  MyDefault(const MyDefault&) = default; // Copy constructor
  MyDefault(MyDefault&&) = default; // Move constructor
  MyDefault& operator=(const MyDefault&) = default; // Copy-assignment operator
  MyDefault& operator=(MyDefault&&) = default; // Move-assignment operator

  // void print(const std::string&) = default;
  int data;
};

int main () {
  MyDefault aDefault;
  std::printf("Without constructor call: MyDefault data initialized to: %d\n", aDefault.data);
  aDefault = MyDefault();
  std::printf("With: MyDefault data initialized to: %d\n", aDefault.data);
  MyDefault newDefault = aDefault;

  MyDefault anotherDefault(static_cast<MyDefault&&>(newDefault));
  return 0;
}