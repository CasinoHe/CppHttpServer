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
    void on_write(bool is_close, boost::beast::error_code ec, std::size_t bytes_transferred);
    void handle_request();

    void do_read();
    template<bool is_request, class Body, class Fields>
    void do_write(boost::beast::http::message<is_request, Body, Fields> &&message);
    void do_close();

  private:
    void handle_bad_request(boost::beast::string_view why);

  private:
    boost::beast::tcp_stream m_stream;
    boost::beast::http::request<boost::beast::http::string_body> m_request;
    boost::beast::flat_buffer m_buffer;
    std::shared_ptr<void> m_response = nullptr;
  };

  template <bool is_request, class Body, class Fields>
  void AsyncTcpConnection::do_write(boost::beast::http::message<is_request, Body, Fields> &&message)
  {
    auto sp = std::make_shared<boost::beast::http::message<is_request, Body, Fields>>(std::move(message));
    m_response = sp;

    boost::beast::http::async_write(m_stream, *sp, 
      boost::beast::bind_front_handler(&AsyncTcpConnection::on_write, shared_from_this(), sp->need_eof()));
  }
}