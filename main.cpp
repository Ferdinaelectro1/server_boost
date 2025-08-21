#include <iostream>
#include <memory>
#include "include/server.h"

std::shared_ptr<Server> server_ptr ;

void on_reveive_handler_client0(std::string& data)
{
   std::string msg = "client 0 : ";
   msg += data;
   server_ptr->send(msg,1);
}

void on_reveive_handler_client1(std::string &data)
{
    std::string msg = "client 1 : ";
    msg += data;
    server_ptr->send(msg,0);
}

void handler_client0()
{
    server_ptr->on_receive(on_reveive_handler_client0,0);
}

void handler_client1()
{
    server_ptr->on_receive(on_reveive_handler_client1,1);
}


int main()
{
    server_ptr = std::make_shared<Server>();
    server_ptr->on_handle(handler_client0,0);
    server_ptr->on_handle(handler_client1,1);
    server_ptr->begin();
    return 0;
}