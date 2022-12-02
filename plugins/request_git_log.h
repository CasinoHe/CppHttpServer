#pragma once
#include "plugin_base.h"
#include <boost/dll.hpp>
#include <string>
#include <vector>
#include <filesystem>

namespace cpp_http_server
{
  class RequestGitLogPlugin : public PluginBase
  {
  public:
    virtual std::string get_plugin_name() override { return "gitlog"; }
    virtual std::string http_request(const std::string &request) override;
    virtual ~RequestGitLogPlugin() {}

  private:
    std::string get_git_log_with_key(const std::string &path, const std::string &key);
    std::vector<std::string> execute_command(const std::string &command, const std::vector<std::string> &args);
    std::filesystem::path m_workpath;
  };

  extern RequestGitLogPlugin plugin;
}

BOOST_DLL_ALIAS(cpp_http_server::plugin, gitlog)