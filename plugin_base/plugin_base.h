#pragma once
#include <boost/config.hpp>
#include <string>

// Warning: 
// This file is the same as plugins/plugin_base.h
// if someone wants to change one of them, should change both of them
namespace cpp_http_server
{
  class BOOST_SYMBOL_VISIBLE  PluginBase
  {
  public:
    // need only a virtual interface
    virtual std::string get_plugin_name() = 0;
    virtual std::string http_request(const std::string &request) = 0;

    virtual ~PluginBase() {}
  };
}