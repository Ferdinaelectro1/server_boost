# MINI BOOST SERVER EN C++
## Bonjour à tous !

### Aujourd’hui, j’ai créé ma propre bibliothèque. Cette bibliothèque permet de gérer des clients avec un serveur web Boost.Asio.

### Utilisation de cette bibliothèque
### Cloner le projet
    mkdir boost_server
    cd boost_server
    git clone https://github.com/Ferdinaelectro1/server_boost.git


## Linux

### Installation de Boost

    sudo apt update
    sudo apt install libboost-all-dev

### Exécuter le serveur

    mkdir build
    chmod +x run
    ./run
### Tester depuis un autre terminal

    nc 127.0.0.1 1234

## Windows

### Installation de Boost via vcpkg

    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    .\bootstrap-vcpkg.bat
    .\vcpkg integrate install
    .\vcpkg install boost-asio

### Installer CMake

    winget install Kitware.CMake

### Exécuter le serveur

    mkdir build
    cd build
    cmake ..
    make
    ./server.exe

### Tester depuis un autre terminal

    nc 127.0.0.1 1234

