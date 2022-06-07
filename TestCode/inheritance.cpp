#include <functional>
#include <iostream>

class A {
 public:
  A() = default;
  ~A() = default;

  virtual void print() { std::cout << "Class A\n"; }
  std::function<void()> fCallback;
};

class B : public A {
 public:
  B() { print(); };
  ~B() = default;
};

class C : public B {
 public:
  C() = default;
  ~C() = default;
  virtual void print() { std::cout << "Class C\n"; }
};

class Holder {
 public:
  Holder(A& inA) : fA(inA) {}
  ~Holder() = default;
  void doPrint() { fA.print(); }

 public:
  A& fA;
};

class BaseDataClass {
 public:
  BaseDataClass() : fData("") {}
  void AppendBaseValue(void) { fData.append("Base value\n"); }
  void Clear() { fData.clear(); }
  std::string fData;
};

class DerivedDataClass : public BaseDataClass {
 public:
  DerivedDataClass() : BaseDataClass() {}
  ~DerivedDataClass() {}
  void AppendDerivedValue() { fData.append("Derived value\n"); }
};

class BaseDriver {
 public:
  BaseDriver() { doThing(); }
  ~BaseDriver() {}
  BaseDataClass fBaseData;

 private:
  void doThing() { std::cout << "BaseDriver thing\n"; }
};

class Driver : public BaseDriver {
 public:
  Driver() : BaseDriver() {
    doThing();
  }
  ~Driver() {}
  DerivedDataClass fDerivedData;
  private:
  void doThing() {
    std::cout << "Parent driver thing\n";
  }
};

int main() {
  C* aClass = new C();
  std::cout << "C Class Print: ";
  aClass->print();

  B* aBClass = dynamic_cast<B*>(aClass);
  std::cout << "B Class Print: ";
  aBClass->print();

  std::printf("%p == %p\n", aClass, aBClass);
  delete aClass;

  // ===============================

  C aCClass;
  aCClass.print();
  aCClass.fCallback = []() {
    std::cout << "Original callback function in C class object\n";
  };

  Holder aHolder(dynamic_cast<A&>(aCClass));
  aHolder.doPrint();

  std::printf("%p == %p\n", &aCClass, &aHolder.fA);
  aCClass.fCallback = []() {
    std::cout << "Overwritten callback function in C class object\n";
  };

  aCClass.fCallback();
  aHolder.fA.fCallback();

  // ===============================
  DerivedDataClass aDerivedClass;
  aDerivedClass.AppendBaseValue();
  aDerivedClass.AppendDerivedValue();
  std::cout << aDerivedClass.fData;

  Driver aDriver;
  return 1;
}
