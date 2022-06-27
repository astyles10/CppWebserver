#include <iostream>
#include <thread>

class thread_lord {
 public:
  thread_lord(std::thread& inThread) : fThread(std::move(inThread)) {}
  ~thread_lord() {
    if (fThread.joinable()) {
      std::cout << "\nJoining thread #" << fThread.get_id()
                << " in thread_lord destructor" << std::endl;
      fThread.join();
    }
  }

 private:
  std::thread fThread;
};

void PrintLine(int inNumber) { std::cout << "Thread created\n"; }

std::thread CreateNewThread() {
  static unsigned long int aThreadNumber = 1;
  std::thread aNewThread(PrintLine, aThreadNumber);
  return std::move(aNewThread);
}

int main() {
  auto aThread = CreateNewThread();
  // Transfer ownership to a thread_lord object
  thread_lord aThreadLord(aThread);
  if (!aThread.joinable()) {
    std::cout << "main: aThread moved successfully\n";
  } else {
    std::cout << "main: aThread not moved??\n";
    aThread.join();
  }

  std::cout << "This should print before destructor\n";
  return 0;
}