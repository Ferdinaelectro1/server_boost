#ifndef _SERVER_H_
#define _SERVER_H_
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>
#include "session.h"


using boost::asio::ip::tcp;

    class Server
    {
        public:
          Server();
          void on_handle(std::function<void()> func,int id);
          void begin();
          void send(const std::string &msg, int id);
          void on_receive(std::function<void(std::string&)> callback,int id);
          int get_client_connect();
          ~Server();

        private:
          boost::asio::io_context _io;
          tcp::acceptor _acceptor;
          std::unordered_map<int,std::shared_ptr<std::function<void()>>> _user_client_handle;
          std::function<void(std::string&)> _user_on_receive_callback;
          std::unordered_map<int,std::shared_ptr<Session>> _sessions;
          void _restart_acceptor();
          int _id;
    };

#endif