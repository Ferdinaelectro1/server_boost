#ifndef __SESSION_H__
#define __SESSION_H__
#include <boost/asio.hpp>
#include <memory>
#include <iostream>

using boost::asio::ip::tcp;

class Session
{
    public:
      Session(tcp::socket);
      void receive_data_handle(std::function<void(std::string&)> callback);
      void send(const std::string &data);
       std::function<void(const boost::system::error_code& error, tcp::socket socket)> server_handle;
       std::function<void()> user_handle;

    private: 
      std::function<void(std::string&)> _receive_callback;
      tcp::socket _socket;
      std::shared_ptr<std::array<char,1024>> _buffer;
      void _do_read();
      
};

#endif