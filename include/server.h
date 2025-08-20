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
          void on_receive(std::function<void(std::string&)> callback);
          ~Server();

        private:
          boost::asio::io_context _io;
          tcp::acceptor _acceptor;
          std::shared_ptr<std::function<void(const boost::system::error_code&)>> _handle_client;
          std::shared_ptr<std::function<void()>> _user_func;
          std::function<void(std::string&)> _user_on_receive_callback;
          tcp::socket _socket;
          std::function<void()> do_read;
          std::shared_ptr<std::array<char,1024>> _read_buffer;
          void _restart_acceptor();
    };

#endif