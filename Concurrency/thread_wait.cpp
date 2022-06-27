#include <unistd.h>

#include <iostream>
#include <thread>

struct func {
  int& i;

  func(int& i_) : i(i_) {}

  void operator()() {
    std::cout << "Running operator func\n";
    for (int j = 0; j < 100000000; ++j) {
      ++i;
    }
  }
};

void valid(int& inValue) {
  func aFunc(inValue);
  std::thread aThread(aFunc);
  aThread.join();
}

void oops(int& inValue) {
  int aValue = 0;
  func aFunc(aValue);
  std::thread aThread(aFunc);

  // Detach separates the thread object from the thread of execution
  // This means the thread object can be deleted before completion
  // If the thread must finish before deletion, join() needs to be called

  // The detached thread continues to run until deleted
  aThread.detach();
  usleep(100000);
  std::cout << "Is joinable? " << aThread.joinable() << "\n";
  std::cout << "aValue = " << aValue << "\n";
}

int main(int argc, char** argv) {
  int aOopsInt = 0;
  std::thread aThread(oops, std::ref(aOopsInt));
  std::cout << aOopsInt << "\n";
  // Causes segfault, thread tries to access deleted data
  // delete aOopsInt;
  aThread.join();
  int aGoodInt(0);
  std::thread aSecondThread(valid, std::ref(aGoodInt));
  // valid(aGoodInt);
  aSecondThread.join();
  std::cout << aGoodInt << "\n";

  return 0;
}