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
 * HEAD /index.html - HEAD just makes a request for a HTTP response header
 * without the body (Like GET without a returned resource)
 *
 * POST /test HTTP/1.1
 * Host: foo.example
 * Content-Type: application/x-www-form-urlencoded
 * Content-Length: 27
 *
 * field1=value1&field2=value2
 *
 * Usually sent by HTML forms
 *
 */

// https://restfulapi.net/http-methods/

HttpParser::HttpParser() {
  _callbacks.emplace("GET", std::bind(&HttpParser::HandleGetRequest, this, std::placeholders::_1));
  _callbacks.emplace("POST", std::bind(&HttpParser::HandlePostRequest, this, std::placeholders::_1));
}

std::string HttpParser::OnData(const std::string &inData) {
  _httpVersion = GetHttpVersion(inData);
  std::string aRequestType = DetermineRequestType(inData);
  std::cout << "OnData received: \n" << inData << "\n";

  const auto &aCallback = _callbacks.find(aRequestType);
  if (aCallback != _callbacks.end()) {
    return CraftResponseMessage(aCallback->second(inData));
  }
  return {};
}

void HttpParser::SetPostRequestHandler(const std::string &inResourcePath,
                                       HttpCallback inCallback) {
  _postCallbacks.emplace(inResourcePath, inCallback);
}

HttpParser::HttpResponse HttpParser::HandleGetRequest(
    const std::string &inRequest) {
  HttpResponse aResponse;
  std::string aRequestPage = DetermineResourceRequest(inRequest, "GET");
  std::cout << "Requested page: \"" << aRequestPage << "\"\n";
  std::cout << "Full request: \n" << inRequest << "\n";

  if (aRequestPage.empty()) {
    aRequestPage = "index.html";
  }

  if (std::filesystem::exists(aRequestPage)) {
    // TODO: Handle based on MIME types
    // https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types
    // Maybe use map of extension to content type?
    const auto &aFileExtension =
        std::filesystem::path(aRequestPage).extension();
    std::ifstream aFileStream;

    if (aFileExtension == ".html") {
      aFileStream.open(aRequestPage);
      aResponse.fContentType = "text/html";
    } else if (aFileExtension == ".ico") {
      aFileStream.open(aRequestPage, std::ios::binary | std::ios::out);
      aResponse.fContentType = "image/x-icon";
    } else {
      aFileStream.open(aRequestPage);
      aResponse.fContentType = "text/plain";
    }
    aResponse.fResponseCode = 200;
    aResponse.fDescription = "OK";
    aResponse.fConnection = "close";

    // Add response codes & descriptions to const map
    std::stringstream aFileBuffer;
    aFileBuffer << aFileStream.rdbuf();
    aResponse.fData = aFileBuffer.str();
  } else {
    aResponse.fResponseCode = 404;
    aResponse.fDescription = "File not found";
    aResponse.fConnection = "close";
    aResponse.fContentType = "text/plain";
    aResponse.fData = "HTTP 404: File not found :(";
  }

  return aResponse;
}

HttpParser::HttpResponse HttpParser::HandlePostRequest(
    const std::string &inRequest) {
  HttpResponse aResponse;

  // POST requests will need to query a set of callbacks to the incoming type
  // E.g. POST /test HTTP/1.1 would need a user defined callback set to handle
  // the incoming request on /test

  const std::string &aRequestPath = DetermineResourceRequest(inRequest, "POST");
  std::cout << "Handling POST request!\n";
  const auto &aCallback = _postCallbacks.find(aRequestPath);
  if (aCallback != _postCallbacks.end()) aResponse = aCallback->second(inRequest);
  // else {
  //   aResponse.fResponseCode = 404;
  //   aResponse.fDescription = "File not found";
  //   aResponse.fConnection = "close";
  //   aResponse.fContentType = "text/plain";
  //   aResponse.fData = "HTTP 404: File not found :(";
  // }

  return aResponse;
}

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

std::string HttpParser::DetermineRequestType(
    const std::string &inRequest) const {
  for (const auto &aType : fRequestTypes)
    if (inRequest.rfind(aType, 0) == 0) return aType;
  return {};
}

std::string HttpParser::DetermineResourceRequest(const std::string &inRequest,
                                                 const std::string &inMethod) {
  // TODO: Prevent directory traversal
  // https://owasp.org/www-community/attacks/Path_Traversal

  unsigned startPosition = inRequest.find(inMethod) + inMethod.length() +
                           1;  // +1 removes leading space character
  unsigned last = inRequest.find(" HTTP");
  std::string aResource = inRequest.substr(startPosition, last - startPosition);

  RemoveTraversalSequences(aResource);

  if (aResource.starts_with("/")) aResource.erase(0, 1);

  return aResource;
}

std::string HttpParser::GetHttpVersion(const std::string &inRequest) {
  std::regex aVersionRegex("HTTP.*");
  std::smatch aMatch;
  std::regex_search(inRequest, aMatch, aVersionRegex);
  return aMatch.str(0);
}

void HttpParser::RemoveTraversalSequences(std::string &inoutResourceRequest) {
  std::size_t aEndPosition = 0;
  while (inoutResourceRequest.find("../", aEndPosition) != std::string::npos)
    aEndPosition += 3;
  inoutResourceRequest = inoutResourceRequest.substr(aEndPosition);
}
