#pragma once
#include <boost/config.hpp>

namespace cpp_http_server
{
  class BOOST_SYMBOL_VISIBLE  PluginBase
  {
  public:
    // need only a virtual interface
    virtual std::string http_request(const std::string &body) = 0;

    virtual ~PluginBase() {}
  };
}