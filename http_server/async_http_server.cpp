#include "async_http_server.h"
#include "async_connection.h"
#include <iostream>
#include <vector>
#include <thread>

namespace cpp_http_server
{
  // std::shared_ptr<boost::asio::io_context> AsyncHttpServer::g_context_ptr = nullptr;

  AsyncHttpServer::AsyncHttpServer(std::string &address, short port, int threads_cnt)
    : m_address(address), m_port(port), m_threads_cnt(threads_cnt)
  {
    if (!AsyncHttpServer::g_context_ptr)
    {
      AsyncHttpServer::g_context_ptr = std::make_shared<boost::asio::io_context>(threads_cnt);
    }
  }

  bool AsyncHttpServer::bind()
  {
    if (!m_acceptor_ptr)
    {
      m_acceptor_ptr = std::make_shared<boost::asio::ip::tcp::acceptor>(boost::asio::make_strand(*AsyncHttpServer::g_context_ptr));
    }

    boost::beast::error_code ec;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(m_address), m_port);
    m_acceptor_ptr->open(endpoint.protocol(), ec);
    if (ec)
    {
      std::cerr << "open listen socket at " << m_address << ":" << m_port << " failed. Reason: " << ec.message() << std::endl;
      return false;
    }

    m_acceptor_ptr->set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec)
    {
      std::cerr << "set listen socket reuse address option failed. " << m_address << ":" << m_port << " Reason: " << ec.message() << std::endl;
      return false;
    }

    m_acceptor_ptr->bind(endpoint, ec);
    if (ec)
    {
      std::cerr << "bind listen socket failed. " << m_address << ":" << m_port << " Reason: " << ec.message() << std::endl;
      return false;
    }
    
    m_acceptor_ptr->listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec)
    {
      std::cerr << "start listen socket failed." << m_address << ":" << m_port << " Reason: " << ec.message() << std::endl;
      return false;
    }

    return true;
  }

  void AsyncHttpServer::do_accept()
  {
    m_acceptor_ptr->async_accept(
      boost::asio::make_strand(*AsyncHttpServer::g_context_ptr),
      boost::beast::bind_front_handler(&AsyncHttpServer::on_accept, shared_from_this())
    );
  }

  void AsyncHttpServer::on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket)
  {
    if (ec)
    {
      std::cerr << "accept failed. Reason: " << ec.message() << std::endl;
      return;
    }
    else
    {
      std::cout << "receive accept event" << std::endl;
      std::make_shared<AsyncTcpConnection>(std::move(socket))->run();
    }

    do_accept();
  }

  bool AsyncHttpServer::start()
  {
    if (!bind())
    {
      return false;
    }

    do_accept();

    std::vector<std::thread> thread_vec;
    thread_vec.reserve(m_threads_cnt);

    for (int i = 0; i < m_threads_cnt; i++)
    {
      thread_vec.emplace_back([&]()
                              {
                                AsyncHttpServer::g_context_ptr->run(); 
                              });
    }

    for (auto &item : thread_vec)
    {
      if (item.joinable())
      {
        item.join();
      }
    }

    return true;
  }
}