#include "../include/session.h"

Session::Session(tcp::socket socket): _socket(std::move(socket)){
    _buffer = std::make_shared<std::array<char,1024>>();
}

void Session::start()
{
    _do_read();
}

void Session::_do_read()
{
    _socket.async_read_some(
        boost::asio::buffer(*_buffer),[&](const boost::system::error_code &err,std::size_t data_length){
            if(!err)
            {
                _do_read();
            }
        }
    );
}