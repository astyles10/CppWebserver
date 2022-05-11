#include <iostream>
#include <vector>

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
  X(const X &other) : data(new int[1000])
  {
    std::copy(other.data, other.data+1000, data);
  }
  X(X&& other): data(other.data)
  {
    other.data = nullptr;
  }
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

int main()
{
  // int& a = 5; // Doesn't compile, lvalue reference can't be made to rvalue

  // Able to compile, a is a reference to a constant value 5
  int const &a = 5;
  std::printf("a=%d\n", a);

  // This is a deliberate exception to allow temporary values to functions containing reference values
  print("Temporary value");

  // i is declared as an rvalue reference to 42
  int &&i = 42;
  std::printf("i is bound to rvalue %d\n", i);
  i = 69;
  std::printf("i changed to %d\n", i);

  return 0;
}