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
  aThread.join();
  aAnotherThread.join();

  int const& i = 69;
}
