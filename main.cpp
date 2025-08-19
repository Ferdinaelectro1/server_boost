#include <iostream>
#include "include/server.h"

int main()
{
    Server server;
    server.on_handle([](){
        std::cout << "Hello ça c'est ma propre implémentation du handle" <<std::endl;
    });
    server.begin();
    return 0;
}