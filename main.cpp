#include <iostream>
#include <memory>
#include "include/server.h"

std::shared_ptr<Server> server_ptr ;

void on_reveive_handler(std::string& data)
{
    std::cout << "New msg : " <<data<<std::endl;
    std::string msg = "Tu as dit : ";
    msg += data;
    //server_ptr->send(msg,"g");
}

void handler()
{
    //std::cout << "Hello ça c'est ma own implémentation du handle" <<std::endl;
    //server_ptr->send("Salut bro c'est Asio. Comment ça va?? \n");
    //server_ptr->on_receive(on_reveive_handler,0);
    server_ptr->send("Client ça va ?",0);
    for(int i=0; i < server_ptr->get_client_connect(); i++)
    {
        server_ptr->send("Client ça va ?",i);
    }
}


int main()
{
    server_ptr = std::make_shared<Server>();
    server_ptr->on_handle(handler,0);
    server_ptr->begin();
    return 0;
}