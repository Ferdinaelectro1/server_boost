#ifndef _SERVER_H_
#define _SERVER_H_
#include <memory>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

    class Server
    {
        public:
          Server();
          void on_handle(std::function<void()> func);
          void begin();
          void send(const std::string &msg);
          ~Server();

        private:
          boost::asio::io_context _io;
          tcp::acceptor _acceptor;
          std::shared_ptr<std::function<void(tcp::socket,const boost::system::error_code&)>> _handle_client;
          std::shared_ptr<std::function<void()>> _user_func;
          void _restart_acceptor();
    };

#endif