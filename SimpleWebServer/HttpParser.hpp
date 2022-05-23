#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include <string>

class HttpParser {
 public:
  HttpParser();
  ~HttpParser();
  std::string OnData(const std::string &inData);
  const std::string &GetData(void) const;

 private:
  std::string DetermineResourceRequest(const std::string &inRequest,
                                       const std::string &inMethod);
  std::string GetHttpVersion(const std::string &inRequest);
  std::string _data;
};

#endif