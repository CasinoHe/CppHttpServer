#include "async_connection.h"
#include <iostream>

namespace cpp_http_server
{
  AsyncTcpConnection::AsyncTcpConnection(boost::asio::ip::tcp::socket &&socket) : m_stream(std::move(socket))
  {

  }

  void AsyncTcpConnection::run()
  {
    boost::asio::dispatch(m_stream.get_executor(),
                          boost::beast::bind_front_handler(&AsyncTcpConnection::do_read, shared_from_this()));
  }

  void AsyncTcpConnection::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
  {
    boost::ignore_unused(bytes_transferred);

    if (ec == boost::beast::http::error::end_of_stream)
    {
      return do_close();
    }

    if (ec)
    {
      std::cerr << "read socket data error, Reason: " << ec.message() << std::endl;
      return;
    }

    handle_request();
  }

  void AsyncTcpConnection::do_read()
  {
    m_request = {};
    boost::beast::http::async_read(m_stream, m_buffer, m_request,
                                   boost::beast::bind_front_handler(&AsyncTcpConnection::on_read, shared_from_this()));
  }

  void AsyncTcpConnection::do_close()
  {
    boost::beast::error_code ec;
    m_stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    if (ec)
    {
      std::cerr << "close socket failed. Reason: " << ec.message() << std::endl;
    }
  }

  boost::beast::http::response<boost::beast::http::string_body> AsyncTcpConnection::handle_bad_request(boost::beast::string_view why)
  {

  }

  void AsyncTcpConnection::handle_request()
  {

  }
}