#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include <string>

class HttpParser {
  typedef struct HttpResponse {
    uint16_t fResponseCode;
    std::string fDescription;
    std::string fContentType;
    std::string fConnection;
    std::string fData;

    HttpResponse() = default;
  } HttpResponse;

 public:
  HttpParser() = default;
  ~HttpParser() = default;
  std::string OnData(const std::string &inData);

 private:
  std::string CraftResponseMessage(const HttpResponse &inResponse) const;
  std::string DetermineResourceRequest(const std::string &inRequest,
                                       const std::string &inMethod);
  std::string GetHttpVersion(const std::string &inRequest);

  std::string _httpVersion;
};

#endif