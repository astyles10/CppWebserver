#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include <array>
#include <functional>
#include <map>
#include <string>

class HttpParser {
  public:
  typedef struct HttpResponse {
    uint16_t fResponseCode;
    std::string fDescription;
    std::string fContentType;
    std::string fConnection;
    std::string fData;

    HttpResponse() = default;
  } HttpResponse;

  typedef std::function<HttpResponse (const std::string&)> HttpCallback;

  HttpParser();
  ~HttpParser() = default;
  std::string OnData(const std::string &inData);
  void SetPostRequestHandler(const std::string& inResourcePath, HttpCallback inCallback);

 private:
  HttpResponse HandleGetRequest(const std::string &inRequest);
  HttpResponse HandlePostRequest(const std::string& inRequest);
  std::string CraftResponseMessage(const HttpResponse &inResponse) const;
  std::string DetermineRequestType(const std::string &inRequest) const;
  std::string DetermineResourceRequest(const std::string &inRequest,
                                       const std::string &inMethod);
  std::string GetHttpVersion(const std::string &inRequest);
  void RemoveTraversalSequences(std::string &inoutResourceRequest);

  const std::array<std::string, 9> fRequestTypes = {
      "CONNECT", "DELETE", "GET", "HEAD", "OPTIONS",
      "PATCH",   "POST",   "PUT", "TRACE"};

  std::string _httpVersion;

  std::map<std::string, HttpCallback> _callbacks;

  std::map<std::string, HttpCallback> _postCallbacks;
};

#endif