#ifndef __SESSION_H__
#define __SESSION_H__
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

class Session
{
    public:
      Session(tcp::socket);
      void start();

    private: 
      tcp::socket _socket;
      std::shared_ptr<std::array<char,1024>> _buffer;
      void _do_read();
      
};

#endif