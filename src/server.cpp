#include "../include/server.h"

Server::Server() : _acceptor(_io,tcp::endpoint(tcp::v4(),1234)),_socket(_io)
{
    //fonction qui serait appelé pour gérer une connexion
    _handle_client = std::make_shared<std::function<void(const boost::system::error_code&)>>();
    _user_func = nullptr;
}

Server::~Server()
{
    _io.stop();
}

//**********Démarrer le serveur************************ */
void Server::begin()
{
    try
    {
        //init de la fonction handleclient
        *_handle_client = [&](const boost::system::error_code& error) {
            if(!error)
            {
                std::cout << "Client connecté" << std::endl;
                (*_user_func)();
            }
            _restart_acceptor();
        };
        _acceptor.async_accept([&](const boost::system::error_code& error, tcp::socket socket){
                std::cout << "passé 1\n";
                _socket = std::move(socket);
                std::cout << "passé 2\n";
                (*_handle_client)(error);
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

/*************Redémarrer acceptor*******************************/
void Server::_restart_acceptor()
{
    _acceptor.async_accept([&](const boost::system::error_code& error, tcp::socket socket){
            _socket = std::move(socket);
            (*_handle_client)(error);
    });   
}

/*************** Fonction de handle d'un client**************** */
void Server::on_handle(std::function<void()> func)
{
    _user_func = std::make_shared<std::function<void()>>(func);
}

/******Envoyer une donnée au client qui s'est connecté**********/
void Server::send(const std::string &msg)
{
    auto buffer = std::make_shared<std::string>(msg);
    boost::asio::async_write(
        _socket,boost::asio::buffer(*buffer),
        [&](const boost::system::error_code& ec,std::size_t) mutable {
        if(!ec)
            std::cout << "Message envoyé !" << std::endl;
        }
    );    
}