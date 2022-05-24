#include "HttpParser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

// HTTP Request messages
// CONNECT, DELETE, GET, HEAD, OPTIONS, PATCH, POST, PUT, TRACE
std::map<std::string, std::function<void()> >fCallbacks;

void HandleGetRequest() {

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

std::string HttpParser::OnData(const std::string &inData) {
  HttpResponse aResponse;
  std::string aRequestPage = DetermineResourceRequest(inData, "GET /");
  _httpVersion = GetHttpVersion(inData);
  if (aRequestPage.empty()) {
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
