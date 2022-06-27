#include <functional>
#include <iostream>
#include <thread>

class background_task {
 public:
  void operator()() const { std::cout << "Doing a background task\n"; }
};

background_task myTask;

void hello() { std::cout << "Hello concurrent world!\n"; }

// std::thread myLastThread(background_task()) {
//   std::thread newThread;
//   return newThread;
// }

std::thread myLastThread(std::function<background_task()> inFunc) {
  return std::thread(inFunc());
}

int main() {
  std::thread aThread(hello);
  std::thread aAnotherThread([]() { std::cout << "Data race\n"; });
  // Can pass a class object with a () operator overload instead of a function
  std::thread aBackgroundTaskThread(myTask);

  const auto aTaskLambda = []() -> background_task {
    background_task aTask;
    return aTask;
  };

  std::thread aLastThread = myLastThread(aTaskLambda);
  std::cout << "Main thread\n";

  aThread.join();
  aAnotherThread.join();
  aBackgroundTaskThread.join();
  aLastThread.join();

  return 0;
}
