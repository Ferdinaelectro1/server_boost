# MINI BOOST SERVER EN C++

## Introduction
Bonjour à tous !
J’ai créé une petite bibliothèque C++ pour gérer des clients via un serveur utilisant **Boost.Asio**.

---

## Cloner le projet
```bash
mkdir boost_server
cd boost_server
git clone https://github.com/Ferdinaelectro1/server_boost.git
```

---

## Linux

### Installer Boost
```bash
sudo apt update
sudo apt install libboost-all-dev
```

### Compiler et exécuter le serveur
```bash
mkdir build
chmod +x run
./run
```

### Tester la communication
Depuis un autre terminal :
```bash
nc 127.0.0.1 1234
```

---

## Windows

### Installer Boost via vcpkg
```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install boost-asio
```

### Installer CMake
```powershell
winget install Kitware.CMake
```

### Compiler et exécuter le serveur
```powershell
mkdir build
cd build
cmake ..
make
./server.exe
```

### Tester la communication
1. Ouvrir deux terminaux :
```powershell
nc 127.0.0.1 1234
```

2. Échanger des messages :
   - Dans le terminal 1 : `Salut terminal 2` → apparaît dans le terminal 2
   - Dans le terminal 2 : `Oui salut terminal 1` → apparaît dans le terminal 1

