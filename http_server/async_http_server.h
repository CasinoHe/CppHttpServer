#pragma once
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <cstddef>
#include <string>
#include <memory>

namespace cpp_http_server
{
  class AsyncHttpServer : public std::enable_shared_from_this<AsyncHttpServer>
  {
  public:
    AsyncHttpServer(std::string &address, short port, int threads_cnt);
    void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);
    bool start();

  public:
    // static variable inline declaration syntax starts from c++17
    inline static std::shared_ptr<boost::asio::io_context> g_context_ptr = nullptr;

  protected:
    bool bind();
    void do_accept();

  private:
    std::string m_address;
    short m_port;
    int m_threads_cnt;

    std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor_ptr = nullptr;
  };
}