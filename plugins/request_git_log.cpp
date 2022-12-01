#include "request_git_log.h"
#include <iostream>

namespace cpp_http_server
{
  RequestGitLogPlugin plugin;

  std::string RequestGitLogPlugin::http_request(const std::string &request)
  {
    return "{\"version\":\"0.0.1\",\"log\":\"{\"data\":\"asdf\"\"}";
  }
}