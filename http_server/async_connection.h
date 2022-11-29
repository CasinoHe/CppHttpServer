#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>

namespace cpp_http_server
{
  class AsyncTcpConnection : public std::enable_shared_from_this<AsyncTcpConnection>
  {
  public:
    AsyncTcpConnection(boost::asio::ip::tcp::socket &&socket);
    void run();

    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
    void handle_request();

    void do_read();
    void do_close();

  private:
    boost::beast::http::response<boost::beast::http::string_body> handle_bad_request(boost::beast::string_view why);

  private:
    boost::beast::tcp_stream m_stream;
    boost::beast::http::request<boost::beast::http::string_body> m_request;
    boost::beast::flat_buffer m_buffer;
  };
}