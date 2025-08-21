#include "../include/session.h"

Session::Session(tcp::socket socket): _socket(std::move(socket)){
    _read_buffer = std::make_shared<std::array<char,1024>>();
    _write_buffer = std::make_shared<std::string>("");
    _receive_callback = nullptr;
    server_handle = [&](const boost::system::error_code& error, tcp::socket socket)
    {
        _socket = std::move(socket);
        if(!error)
        {
            std::cout << "\nSession démarré"<< std::endl;
            if(user_handle != nullptr)
            {
                user_handle();
            }
        }
        else
        {
            std::cerr << "Erreur lors de l'acceptation du client"<< std::endl;
        }
    };
}


void Session::_do_read()
{
    _socket.async_read_some(
        boost::asio::buffer(*_read_buffer),[&](const boost::system::error_code &err,std::size_t data_length){
            if(!err)
            {
                std::string msg(_read_buffer->data(),data_length);
                if(_receive_callback != nullptr)
                {
                    _receive_callback(msg);
                }
                _do_read();
            }
        }
    );
}

void Session::receive_data_handle(std::function<void(std::string&)> callback)
{
    _receive_callback = callback;
    _do_read();
}

void Session::send(const std::string &data)
{
    *_write_buffer = data;
    boost::asio::async_write(
        _socket,boost::asio::buffer(*_write_buffer),
        [&](const boost::system::error_code& ec,std::size_t) {
            if(!ec){
                std::cout << "Message envoyé !" << std::endl;
            }
            else
            {
                std::cerr << "Client déconnecté" << std::endl;
            }
        }
    );
}