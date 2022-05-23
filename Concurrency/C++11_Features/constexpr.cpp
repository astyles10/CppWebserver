#include <iostream>
#include <set>
#include <tuple>
#include <type_traits>
#include <iterator>
#include <vector>

template <typename T>
class List {
 public:
  List() = default;
  List& operator=(const List& other) = default;
  ~List() = default;

  bool Append(T inItem) {
    auto ret = fData.emplace(inItem);
    return ret.second;
  }

  template <class Q = T>
  typename std::enable_if<std::is_literal_type<Q>::value, bool>::type PrintItems(void) {
    for (const auto& item : fData) {
      std::cout << item << "\n";
    }
    return true;
  }

  template <class Q = T>
  typename std::enable_if<std::_RequireInputIter<Q>::value, bool> PrintVectorItems(void) {
    for (const auto& item : fData) {
      for (const auto subItem : item) {
        std::cout << subItem << "\n";
      }
    }
    return true;
  }

 private:
  std::set<T> fData;
};

typedef struct structure {
  int x;
  int y;
  structure() : x(0), y(0) {}
  structure(int inX, int inY) : x(inX), y(inY) {}

} structure;

bool operator<(const structure& inLhs, const structure& inRhs) {
  return std::tie(inLhs.x, inLhs.y) < std::tie(inRhs.x, inRhs.y);
}

int main() {
  List<int> aList;
  aList.Append(42);
  aList.PrintItems();

  List<double> aDoubleList;
  aDoubleList.Append(42.5);
  aDoubleList.PrintItems();

  List<std::string> aNewList;
  aNewList.Append("Forty-two");

  List<structure> aStructList;
  structure str(4, 2);
  structure rts(6, 9);
  str < rts ? printf("smaller\n") : printf("larger\n");
  aStructList.Append(str);
  // aStructList.PrintItems();
  List<std::vector<int> > aVectorList;
  std::vector<int> aVec = {1, 2, 3, 4};
  aVectorList.Append(aVec);
  // aVectorList.PrintVectorItems();
  List<std::string>* listPtr = new List<std::string>();
  listPtr->Append("pointer list");
  delete listPtr;
  return 0;
}