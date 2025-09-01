#include "../include/server.h"

Server::Server() : _acceptor(_io,tcp::endpoint(tcp::v4(),1234))
{
    _id = 0;
    //fonction qui serait appelé pour gérer une connexion
    _user_client_handle[_id] = nullptr;
    _user_on_receive_callback = nullptr;
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
        _restart_acceptor();
        std::cout << "Serveur Démarré"<<std::endl;
        _io.run();
    }
    catch(const std::exception& e)
    {
        std::cerr <<"Erreur : "<< e.what() << '\n';
    }
   
}

//fonction handle a appeler pour chaque client
void Server::on_handle(std::function<void()> func,int id)
{
    _user_client_handle[id] = std::make_shared<std::function<void()>>(func);
}


/*************Redémarrer acceptor*******************************/
void Server::_restart_acceptor()
{
/*     _acceptor.async_accept([&](const boost::system::error_code& error, tcp::socket socket){
            _sessions[_id] = std::make_shared<Session>(tcp::socket(_io));
            if(_user_client_handle[_id] != nullptr)
            {
                _sessions[_id]->user_handle = *_user_client_handle[_id];   
            } 
            _sessions[_id]->server_handle(error,std::move(socket)); 
            std::cout << "Client : "<<_id<<" connecté"<< std::endl;
            _restart_acceptor();
            _id++;
    }); */
    _acceptor.async_accept([&](const boost::system::error_code& error, tcp::socket socket){
            _work_session = std::make_shared<Session>(tcp::socket(_io));
            //handle pour gérer un client qui se connecte
            //Dans ce handle , on implémente la fonction de réception du message du client, 
            // en faite le client va envoyé en premier son id, une fois l'id reçu,
            //on crée crée une session avec pour clé l'id du client 
            _work_session->user_handle = [&](){
                _work_session->receive_data_handle([&](const std::string &msg_receive){
                    int this_client_id = std::stod(msg_receive);
                    std::cout << "Client ID : "<<this_client_id<<std::endl;
                    _work_session->send("OK");
                    _sessions[this_client_id] = std::make_shared<Session>(tcp::socket(_io));
                    _sessions[this_client_id] = std::move(_work_session);
                    if(_user_client_handle[this_client_id] != nullptr)
                    {
                        _sessions[this_client_id]->user_handle = *_user_client_handle[this_client_id];   
                    }  
                    _work_session = nullptr;
                });
            };
           _work_session->server_handle(error,std::move(socket)); 
            std::cout << "Client : "<<_id<<" connecté"<< std::endl;
            _restart_acceptor();
    }); 
}



/******Envoyer une donnée au client qui s'est connecté**********/
void Server::send(const std::string &msg,int id)
{    
    if(_sessions.find(id) != _sessions.end())
      _sessions[id]->send(msg); 
    else
      std::cerr << "Ce client n'existe pas "<< std::endl; 
}

//Fonction qui lit une chaine envoyé par le client
void Server::on_receive(std::function<void(std::string&)> callback,int id)
{
    _sessions[_id]->receive_data_handle(callback);
}

//Nombre total de client connecté
int Server::get_client_connect()
{
    return _id+1;
}