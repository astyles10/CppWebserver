#include <functional>
#include <iostream>

class StringHolder {
 public:
  StringHolder() = default;
  ~StringHolder() = default;
  bool CompareStrings(const std::string& inString) const {
    std::printf("Comparing %s to %s\n", inString.c_str(), fStringy.c_str());
    return inString == fStringy;
  }

 private:
  const std::string& fStringy = "StringHolder string";
};

class StringGetter {
 public:
  StringGetter(StringHolder& inHolder) {
    const auto aLamb = [&, inHolder](const std::string& inStr) -> bool {
      return inHolder.CompareStrings(inStr);
    };
    fCallback = aLamb;
  }
  ~StringGetter() = default;

  bool DoCallback(const std::string& inStr) const {
    return fCallback(inStr);
  }

  std::function<bool(const std::string&)> fCallback;
};

int main() {
  StringHolder aHolder;
  StringGetter aGetter(aHolder);

  aGetter.DoCallback("StringHolder string");
  aGetter.DoCallback("kewwe");
  return 0;
}