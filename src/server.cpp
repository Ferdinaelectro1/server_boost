#include "../include/server.h"

Server::Server() : _acceptor(_io,tcp::endpoint(tcp::v4(),1234))
{
    //fonction qui serait appelé pour gérer une connexion
    _handle_client = std::make_shared<std::function<void(tcp::socket,const boost::system::error_code&)>>();
    _user_func = nullptr;
    *_handle_client = [&](tcp::socket socket,const boost::system::error_code& error) {
        if(!error)
        {
            std::cout << "Client connecté" << std::endl;
            std::shared_ptr socket_ptr = std::make_shared<tcp::socket>(std::move(socket));
            std::shared_ptr msg = std::make_shared<std::string>();
            *msg = "Salut depuis boost.asio\n";
            boost::asio::async_write(
                *socket_ptr,boost::asio::buffer(*msg),
                [&](const boost::system::error_code& ec,std::size_t) mutable {
                if(!ec)
                    std::cout << "Message envoyé !" << std::endl;
                    (*_user_func)();
                }
            );   
        }
        _restart_acceptor();
    };
}

Server::~Server()
{
    _io.stop();
}

void Server::begin()
{
    try
    {
        _acceptor.async_accept([&](const boost::system::error_code& error, tcp::socket socket){
                (*_handle_client)(std::move(socket),error);
        });
        if(_user_func == nullptr)
        {
            _user_func = std::make_shared<std::function<void()>>([&](){
                std::cout << "Fonction handle par défaut "<< std::endl;
            });       
        }
        std::cout << "Serveur Démarré"<<std::endl;
        _io.run();
    }
    catch(const std::exception& e)
    {
        std::cerr <<"Erreur : "<< e.what() << '\n';
    }
    
}

void Server::_restart_acceptor()
{
    _acceptor.async_accept([&](const boost::system::error_code& error, tcp::socket socket){
            (*_handle_client)(std::move(socket),error);
    });   
}

void Server::on_handle(std::function<void()> func)
{
    _user_func = std::make_shared<std::function<void()>>(func);
}