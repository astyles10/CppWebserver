#include "HttpParser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

HttpParser::HttpParser() : _data({}) {}

HttpParser::~HttpParser() {}

const std::string &OkResponse =
    "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nConnection : "
    "close\r\n\r\n";

std::string HttpParser::OnData(const std::string &inData) {
  const std::string aRequestPage = DetermineResourceRequest(inData, "GET /");
  std::cout << "Http Version: " << GetHttpVersion(inData)
            << "\nRequested page: \"" << aRequestPage << "\"\n";

  if (aRequestPage.empty()) {
    std::ifstream aFileStream("index.html");
    std::stringstream aBuffer;
    aBuffer << OkResponse;
    aBuffer << aFileStream.rdbuf() << "\r\n\r\n";

    return aBuffer.str();
  } else if (std::filesystem::exists(aRequestPage)) {
    std::ifstream aFileStream(aRequestPage, std::ios::binary | std::ios::out);
    std::stringstream aBuffer;
    aBuffer << "HTTP/1.1 200 OK\r\nContent-Type: image/xicon; Connection: close\r\n\r\n";
    aBuffer << aFileStream.rdbuf() << "\r\n\r\n";

    return aBuffer.str();
  }
  return {};
}

std::string HttpParser::DetermineResourceRequest(const std::string &inRequest,
                                                 const std::string &inMethod) {
  unsigned first = inRequest.find(inMethod);
  unsigned startPosition = first + inMethod.length();
  unsigned last = inRequest.find(" HTTP");
  return inRequest.substr(startPosition, last - startPosition);
}

std::string HttpParser::GetHttpVersion(const std::string &inRequest) {
  std::regex aVersionRegex("HTTP.*");
  std::smatch aMatch;
  std::regex_search(inRequest, aMatch, aVersionRegex);
  return aMatch.str(0);
}

const std::string &HttpParser::GetData(void) const { return _data; }