#include "http_server/async_http_server.h"
#include "plugin_base/plugin_mgr.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

int main(int argc, char **argv)
{
  namespace bp = boost::program_options;

  bp::options_description desc("All options");
  std::vector<std::string> with_plugis;
  std::string ip_address;
  short port;
  int threads;

  desc.add_options()
    ("help,h", "show help message")
    ("with,w", bp::value<std::vector<std::string>>(&with_plugis)->multitoken(), "with plugins, for example: --with=gitlog --with=inner_list")
    ("address", bp::value<std::string>(&ip_address)->default_value("0.0.0.0"), "http server ip, default is 0.0.0.0")
    ("port", bp::value<short>(&port)->default_value(80), "http server port, default is 80")
    ("threads", bp::value<int>(&threads)->default_value(10), "http server threads number, default is 10");
  bp::variables_map vm;

  try
  {
    bp::store(bp::parse_command_line(argc, argv, desc), vm);
  }
  catch(...)
  {
    std::cout << "Parse argument failed." << std::endl;
    std::cout << desc << std::endl;
    return EXIT_FAILURE;
  }

  bp::notify(vm);

  if (vm.count("help") > 0)
  {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  if (vm.count("with") <= 0)
  {
    std::cout << "CppHttpServer Starts with no plugins, finihed all tasks and exit normally." << std::endl;
    return EXIT_SUCCESS;
  }

  auto plugin_mgr = cpp_http_server::PluginManager::get_instance();
  bool regist_succeed = true;
  for (auto &item : with_plugis)
  {
    if (!plugin_mgr->regist_plugin(item))
    {
      std::cout << "regist plugin " << item << " failed." << std::endl;
      regist_succeed = false;
    }
  }

  if (!regist_succeed)
  {
    return EXIT_FAILURE;
  }

  // start http server
  auto http_server = std::make_shared<cpp_http_server::AsyncHttpServer>(ip_address, port, threads);
  bool result = http_server->start();
  if (result)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}