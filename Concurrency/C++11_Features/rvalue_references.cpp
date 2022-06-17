#include <iostream>
#include <vector>
#include <cxxabi.h>
#include <type_traits>
#include <memory>
#include <mutex>

class X
{
private:
  int *data;

public:
  X() : data(new int[1000])
  {}
  ~X()
  {
    delete[] data;
  }
  // Allocates a chunk of memory for new object, then copies data
  X(const X &other) : data(new int[1000])
  {
    std::copy(other.data, other.data+1000, data);
  }
  // Move semantics: swaps the pointer to data from other object to new object
  X(X&& other): data(other.data)
  {
    other.data = nullptr;
  }
};

class Y : public X {
 public:
  Y() : X() {}
  Y(const Y&) = delete;
  Y& operator=(const Y&) = delete;
  ~Y() {}
  static const int val = 5;
};

class MoveOnly {
  std::unique_ptr<int> data;
 public:
  MoveOnly() {}

  MoveOnly(const Y&) = delete;
  MoveOnly(MoveOnly&& other) {
    data = std::move(other.data);
  }

  MoveOnly& operator=(const MoveOnly&) = delete;
  MoveOnly& operator=(MoveOnly&& other) {
    data = std::move(other.data);
    return *this;
  }
  ~MoveOnly() {}
 private:
};

void print(std::string const &line)
{
  std::cout << line << " printed\n";
}

// Makes a copy of incoming vector
void process_copy(const std::vector<int> &toCopy)
{
  std::vector<int> aVec(toCopy);
  aVec.push_back(73);
}

void process_copy(std::vector<int> &&toCopy)
{
  toCopy.push_back(69);
}

void thingy(X&& inObj)
{
  X a(inObj);
  X b(std::move(inObj));
}

template<typename T>
void foo(T&& t)
{
  char * name = abi::__cxa_demangle(typeid(t).name(), 0, 0, NULL);
  printf("type is %s\n", name);
  free(name);
}

int main()
{
  // int& a = 5; // Doesn't compile, lvalue reference can't be made to rvalue

  // Able to compile, a is a reference to a constant value 5
  int const &a = 5;
  std::printf("a=%d\n", a);

  // This is a deliberate exception to allow temporary values to functions containing reference values
  print("Temporary value");
  const std::string& aPermValue = "Permanent value";
  print(aPermValue);

  // i is declared as an rvalue reference to 42
  int &&i = 42;
  std::printf("i is bound to rvalue %d\n", i);
  i = 69;
  std::printf("i changed to %d\n", i);

  // Objects can be cast to an rvalue by using std::move() or static_cast<type>()
  X x1;
  X x2 = std::move(x1);
  X x3 = static_cast<X&&>(x2);

  thingy(X());
  // Won't compile: rvalue reference cannot be bound to an lvalue
  // thingy(x1);

  foo(5);
  foo(3.14);
  foo("blah");
  foo(X());

  Y y1;

  // Cannot reference deleted functions
  // Y y2(y1);
  // Y y3 = y1;

  std::printf("Y is convertible to X: %s\n", std::is_convertible<Y, X>().value ? "true" : "false");
  std::printf("X is convertible to Y: %s\n", (std::is_convertible<X, Y>().value ? "true" : "false"));
  std::mutex aMutex;

  MoveOnly aMoveOnly, aMoveOnly2;

  aMoveOnly = static_cast<MoveOnly&&>(aMoveOnly2);
  return 0;
}