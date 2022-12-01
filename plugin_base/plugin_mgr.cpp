#include "plugin_mgr.h"
#include "plugin_base.h"
#include <boost/dll.hpp>
#include <filesystem>
#include <iostream>

namespace cpp_http_server
{
  std::shared_ptr<PluginManager> PluginManager::get_instance()
  {
    static std::shared_ptr<PluginManager> ptr;
    
    if (!ptr)
    {
      ptr = std::make_shared<PluginManager>();
    }

    return ptr;
  }

  PluginManager::PluginManager()
  {

  }

  bool PluginManager::regist_plugin(const std::string &plugin_path)
  {
    if (!std::filesystem::exists(plugin_path))
    {
      std::cerr << "cannot find plugin: " << plugin_path << std::endl;
      return false;
    }

    if (m_plguins.find(plugin_path) != m_plguins.end())
    {
      // TODO: uload plugin and register again
      return false;
    }

    boost::dll::fs::path lib_path(plugin_path);
    boost::shared_ptr<PluginBase> plugin_ptr;

    plugin_ptr = boost::dll::import_symbol<PluginBase>(lib_path, plugin_path, boost::dll::load_mode::append_decorations);
    m_plguins[plugin_path] = plugin_ptr;

    return true;
  }
}