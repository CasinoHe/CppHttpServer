#include "plugin_mgr.h"
#include "plugin_base.h"
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

  boost::shared_ptr<PluginBase> PluginManager::get_plugin(const std::string &plugin_name)
  {
    auto item = m_plugins.find(plugin_name);
    if (item != m_plugins.end())
    {
      return item->second;
    }
    else
    {
      return nullptr;
    }
  }

  bool PluginManager::regist_plugin(const std::string &plugin_path)
  {
    if (m_plugins.find(plugin_path) != m_plugins.end())
    {
      // TODO: uload plugin and register again
      return false;
    }

    boost::shared_ptr<PluginBase> plugin = nullptr;
    try
    {
      plugin = boost::dll::import_alias<PluginBase>(plugin_path, plugin_path, boost::dll::load_mode::append_decorations);
    }
    catch(std::exception &e)
    {
      std::cout << "load shared library " << plugin_path << " failed. Reason: " << e.what() << std::endl;
      return false;
    }
    
    std::cout << "load shared library " << plugin_path << " succeed!" << std::endl;
    m_plugins[plugin_path] = plugin;

    return true;
  }
}