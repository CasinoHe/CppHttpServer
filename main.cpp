#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char **argv)
{
  namespace bp = boost::program_options;

  bp::options_description desc("All options");
  std::vector<std::string> with_plugis;

  desc.add_options()
    ("help,h", "show help message")
    ("with,w", bp::value<std::vector<std::string>>(&with_plugis)->multitoken(), "with plugins, for example: --with=gitlog --with=inner_list");
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

  std::vector<std::string> all_plugins;
  if (vm.count("with") <= 0)
  {
    std::cout << "CppHttpServer Starts with no plugins, finihed all tasks and exit normally." << std::endl;
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}