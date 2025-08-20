#include <iostream>
#include <memory>
#include "include/server.h"

std::shared_ptr<Server> server_ptr ;

void handler()
{
    std::cout << "Hello ça c'est ma own implémentation du handle" <<std::endl;
    server_ptr->send("Salut bro c'est Asio. Comment ça va?? \n");
}

int main()
{
    server_ptr = std::make_shared<Server>();
    server_ptr->on_handle(handler);
    server_ptr->begin();
    return 0;
}