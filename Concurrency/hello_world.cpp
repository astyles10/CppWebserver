#include <iostream>
#include <thread>

void hello()
{
  std::cout << "Hello concurrent world!\n";
}

int main ()
{
  std::thread aThread(hello);
  std::thread aAnotherThread([](){std::cout << "Data race\n";});
  std::cout << "Main thread\n";
  aThread.join();
  aAnotherThread.join();
}
