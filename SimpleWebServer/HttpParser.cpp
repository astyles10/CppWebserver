#include "HttpParser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

// HTTP Request messages
// CONNECT, DELETE, GET, HEAD, OPTIONS, PATCH, POST, PUT, TRACE

/* 
 * 
 * CONNECT server.example.com:80 HTTP/1.1
 * Host: server.example.com:80
 * Proxy-Authorization: basic aGVsbG86d29ybGQ=
 * 
 * DELETE /file.html HTTP/1.1
 * Host: example.com
 * 
 * GET /index.html
 * 
 * 
 */




std::map<std::string, std::function<void()> > fCallbacks;

void HandleGetRequest(const std::string &inRequest) {}

std::string HttpParser::CraftResponseMessage(
    const HttpResponse &inResponse) const {
  std::stringstream aResponseBuffer;
  aResponseBuffer << _httpVersion << " " << inResponse.fResponseCode << " "
                  << inResponse.fDescription
                  << "\r\nContent-Type: " << inResponse.fContentType
                  << " Connection: " << inResponse.fConnection << "\r\n\r\n"
                  << inResponse.fData << "\r\n\r\n";

  return aResponseBuffer.str();
}

std::string HttpParser::OnData(const std::string &inData) {
  HttpResponse aResponse;
  const std::string& aRequestType = DetermineRequestType(inData);
  // TODO: Ensure directory traversal not possible
  std::string aRequestPage = "./" + DetermineResourceRequest(inData, aRequestType);
  std::cout << "Requested page: \"" << aRequestPage << "\"\n";
  _httpVersion = GetHttpVersion(inData);
  if (aRequestPage == "./") {
    aRequestPage = "index.html";
  }

  if (std::filesystem::exists(aRequestPage)) {
    // TODO: Handle based on MIME types
    // https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types
    const auto aFileExtension = std::filesystem::path(aRequestPage).extension();
    if (aFileExtension == ".html") {
      std::ifstream aFileStream(aRequestPage);
      aResponse.fResponseCode =
          200;  // Add response codes & descriptions to const map
      aResponse.fDescription = "OK";
      aResponse.fContentType = "text/html";
      aResponse.fConnection = "close";

      std::stringstream aFileBuffer;
      aFileBuffer << aFileStream.rdbuf();
      aResponse.fData = aFileBuffer.str();
    } else if (aFileExtension == ".ico") {
      std::ifstream aFileStream(aRequestPage, std::ios::binary | std::ios::out);
      aResponse.fResponseCode = 200;
      aResponse.fDescription = "OK";
      aResponse.fContentType = "image/x-icon";
      aResponse.fConnection = "close";
      std::stringstream aFileBuffer;
      aFileBuffer << aFileStream.rdbuf();
      aResponse.fData = aFileBuffer.str();
    } else {
      std::ifstream aFileStream(aRequestPage);
      aResponse.fResponseCode = 200;
      aResponse.fDescription = "OK";
      aResponse.fContentType = "text/plain";
      aResponse.fConnection = "close";
      std::stringstream aFileBuffer;
      aFileBuffer << aFileStream.rdbuf();
      aResponse.fData = aFileBuffer.str();
    }
  } else {
    aResponse.fResponseCode = 404;
    aResponse.fDescription = "File not found";
    aResponse.fConnection = "close";
    aResponse.fContentType = "text/plain";
    aResponse.fData = "HTTP 404: File not found :(";
  }
  return CraftResponseMessage(aResponse);
}

std::string HttpParser::DetermineRequestType(
    const std::string &inRequest) const {
  for (const auto &aType : fRequestTypes)
    if (inRequest.rfind(aType, 0) == 0) return aType;
  return {};
}

std::string HttpParser::DetermineResourceRequest(const std::string &inRequest,
                                                 const std::string &inMethod) {
  // TODO: Prevent directory traversal https://owasp.org/www-community/attacks/Path_Traversal
  unsigned first = inRequest.find(inMethod) + 1;
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
