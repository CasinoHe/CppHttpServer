#pragma once
#include "plugin_base.h"
#include <boost/dll.hpp>
#include <string>

namespace cpp_http_server
{
  class RequestGitLogPlugin : public PluginBase
  {
  public:
    virtual std::string get_plugin_name() override { return "gitlog"; }
    virtual std::string http_request(const std::string &request) override;
    virtual ~RequestGitLogPlugin() {}
  };

  extern RequestGitLogPlugin plugin;
}

BOOST_DLL_ALIAS(cpp_http_server::plugin, gitlog)