#include <functional>
#include <map>
#include <string>

class RequestHandler {
 public:
  RequestHandler() = default;
  ~RequestHandler() = default;
  void Register();
  void Execute();

 private:
  std::map<std::string, std::function<std::string(const std::string&)> > _callbacks;
};