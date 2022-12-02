#include "request_git_log.h"
#include <boost/process.hpp>
#include <iostream>
#include <filesystem>

namespace cpp_http_server
{
  RequestGitLogPlugin plugin;

  std::string RequestGitLogPlugin::http_request(const std::string &request)
  {
    m_workpath = std::filesystem::current_path();
    std::string result;
    try
    {
      std::filesystem::path git_path(m_workpath);
      result = get_git_log_with_key(git_path.parent_path().parent_path().generic_string(), "base");
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return "";
    }
    
    std::filesystem::current_path(m_workpath);
    return result;
  }

  std::string RequestGitLogPlugin::get_git_log_with_key(const std::string &path, const std::string &key)
  {
    // change working directory
    if (!std::filesystem::exists(path) && !std::filesystem::is_directory(path))
    {
      return {};
    }
    std::filesystem::current_path(path);

    std::string search_key("--grep=" + key);
    std::vector<std::string> args{"log", "--name-status", search_key};
    std::vector<std::string> v_result = execute_command("git", args);
    std::string result;

    for (auto &item : v_result)
    {
      result += item + "\n";
    }
    return result;
  }

  std::vector<std::string> RequestGitLogPlugin::execute_command(const std::string &command, const std::vector<std::string> &args)
  {
    boost::process::ipstream is;

    auto exe_path(boost::process::search_path(command));
    if (exe_path.empty())
    {
      std::cerr << "cannot find exe path " << command << std::endl;
      return {};
    }

    boost::process::child process(exe_path, boost::process::args(args), boost::process::std_out > is);

    std::vector<std::string> out_data;
    std::string line;

    while(process.running())
    {
      if (std::getline(is, line) && !line.empty())
      {
        out_data.push_back(line);
      }
    }

    process.wait();

    return out_data;
  }
}