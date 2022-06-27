#include <signal.h>

#include <iostream>
#include <memory>
#include <thread>

class Foo {
 private:
  int m_foo;

 public:
  // Defining Foo(int) constructor as explicit will prevent the compiler from
  // performing implicit conversions of type int to Foo
  Foo(int foo) : m_foo(foo) {}
  int GetFoo() { return m_foo; }
};

void DoBar(Foo foo) {
  int i = foo.GetFoo();
  std::printf("DoBar: i=%d\n", i);
}

class thread_guard {
 public:
  explicit thread_guard(std::thread& inThread) : t(inThread) {}
  ~thread_guard() {
    if (t.joinable()) t.join();
  }
  thread_guard(const thread_guard&) = delete;
  thread_guard& operator=(thread_guard const&) = delete;

 private:
  std::thread& t;
};

struct func {
  int& i;

  func(int& i_) : i(i_) {}

  void operator()() {
    std::cout << "Running operator func\n";
    for (int j = 0; j < 100000000; ++j) {
      ++i;
    }
    doSomething();
  }

  void doSomething() {
    int aNumber = std::atoi("1234");
    std::printf("Converted number %d\n", aNumber);
  }
};

void f(int inVal, int& inAnotherVal) {
  func my_func(inAnotherVal);
  std::thread aThread(my_func);
  thread_guard aGuard(aThread);
  int i = 10;
  try {
    i /= inVal;
  } catch (std::logic_error err) {
    std::cerr << err.what() << "\n";
  }
}

int main(int argc, char** argv) {
  std::shared_ptr<void(int)> handler(
      signal(SIGFPE,
             [](int signum) {
               throw std::logic_error("Floating point error caught!");
             }),
      [](__sighandler_t f) { signal(SIGFPE, f); });
  int i = 0;
  f(0, i);

  std::printf("i = %d\n", i);

  DoBar(42);
  return 0;
}