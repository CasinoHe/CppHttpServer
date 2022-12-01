#pragma once
#include <boost/dll.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <memory>

namespace cpp_http_server
{
  class PluginBase;

  // plugin is a singleton
  class PluginManager
  {
  public:
    PluginManager(PluginManager &) = delete;
    PluginManager(PluginManager &&) = delete;
    PluginManager &operator=(PluginManager &) = delete;
    PluginManager &&operator=(PluginManager &&) = delete;
    PluginManager();

  public:
    static std::shared_ptr<PluginManager> get_instance();

    bool regist_plugin(const std::string &plugin_path);
    boost::shared_ptr<PluginBase> get_plugin(const std::string &plugin_name);

  private:
    std::map<std::string, boost::shared_ptr<PluginBase>> m_plugins;
  };
}