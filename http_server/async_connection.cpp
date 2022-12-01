#include "async_connection.h"
#include "plugin_base/plugin_mgr.h"
#include "plugins/plugin_base.h"
#include <iostream>
#include <filesystem>

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

  void AsyncTcpConnection::handle_bad_request(boost::beast::string_view why)
  {
    boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::bad_request, m_request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());
    res.body() = std::string(why);
    res.prepare_payload();

    return do_write(std::move(res));
  }

  void AsyncTcpConnection::handle_request()
  {
    if (m_request.method() != boost::beast::http::verb::get &&
        m_request.method() != boost::beast::http::verb::post &&
        m_request.method() != boost::beast::http::verb::head)
    {
      return handle_bad_request("Unknown HTTP-method");
    }

    if (m_request.target().empty() ||
        m_request.target()[0] != '/' ||
        m_request.target().find("..") != boost::beast::string_view::npos)
    {
      return handle_bad_request("Illegal request-target");
    }

    // Respond to HEAD request
    if(m_request.method() == boost::beast::http::verb::head)
    {
      boost::beast::http::response<boost::beast::http::empty_body> res{boost::beast::http::status::ok, m_request.version()};
      res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
      res.set(boost::beast::http::field::content_type, "application/text");
      res.content_length(0);
      res.keep_alive(m_request.keep_alive());
      return do_write(std::move(res));
    }

    std::filesystem::path url_path(m_request.target().to_string().substr(1));

    // use the first directory name as the plugin key name
    std::string plugin_name((*url_path.begin()).generic_string());

    // find plugin from plugin manager
    auto plugin = PluginManager::get_instance()->get_plugin(plugin_name);
    if (!plugin)
    {
      return handle_bad_request("Illegal request-target");
    }

    // push the request to the plugin, and get a response write back to client
    std::string response;
    try
    {
      response = plugin->http_request(url_path.generic_string());
    }
    catch (std::exception &e)
    {
      std::cerr << "execute plugin's http request failed." << std::endl;
      return handle_bad_request("Internal Error.");
    }

    if (response.empty())
    {
      return handle_bad_request("Internal Request handle failed");
    }

    boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::ok, m_request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());
    res.body() = response;
    res.prepare_payload();

    return do_write(std::move(res));
  }

  void AsyncTcpConnection::on_write(bool is_close, boost::beast::error_code ec, std::size_t bytes_transferred)
  {
    if (ec)
    {
      std::cerr << "read socket data error, Reason: " << ec.message() << std::endl;
      return;
    }

    if (is_close)
    {
      return do_close();
    }

    m_response = nullptr;
    do_read();
  }
}