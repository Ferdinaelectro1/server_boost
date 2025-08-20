#include "../include/server.h"

Server::Server() : _acceptor(_io,tcp::endpoint(tcp::v4(),1234)),_socket(_io)
{
    //fonction qui serait appelé pour gérer une connexion
    _handle_client = std::make_shared<std::function<void(const boost::system::error_code&)>>();
    _user_func = nullptr;
    _user_on_receive_callback = nullptr;
    _read_buffer = std::make_shared<std::array<char,1024>>();
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
                _socket = std::move(socket);
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

//Fonction qui lit une chaine envoyé par le client
void Server::on_receive(std::function<void(std::string&)> callback)
{
    _user_on_receive_callback = callback;
    do_read = [&](){_socket.async_read_some(
        boost::asio::buffer(*_read_buffer),[&](const boost::system::error_code& ec, std::size_t bytes_transferred) mutable {
            if(!ec)
            {
                std::string data_receive(_read_buffer->data(),bytes_transferred); 
                if(_user_on_receive_callback != nullptr)
                   _user_on_receive_callback(data_receive);
                do_read();
                //std::cout << "Reçu du client : "<<data_receive << std::endl;*/
            }
        }
      );
    };
    do_read();
}