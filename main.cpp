#include <iostream>
#include "include/server.h"

void handler()
{
    std::cout << "Hello ça c'est ma propre implémentation du handle" <<std::endl;
}

int main()
{
    Server server;
    server.on_handle(handler);
    server.begin();
    return 0;
}