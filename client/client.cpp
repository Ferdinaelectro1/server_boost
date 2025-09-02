#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <fstream>

std::vector<unsigned char> get_pdf_data()
{
    std::ifstream pdf_file("test.pdf",std::ios::binary);
    if(!pdf_file)
    {
        std::cerr << "Impossible d'ouvrir le fichier test.pdf"<<std::endl;
        exit(1);
    }
    std::vector<unsigned char> pdf_data((std::istreambuf_iterator<char>(pdf_file)),std::istreambuf_iterator<char>());
    pdf_data.push_back('\n');
    return pdf_data;
}

int main(int argc, char *argv[]) {
    if(argc < 3)
    {
        std::cerr << "Usage : ./client your_id dest_id"<<std::endl;
        return 1;
    }
    std::string your_id = argv[1]; 
    std::string dest_id = argv[2];
    try {
        boost::asio::io_context io_context;

        // Résolution du serveur et port
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345"); // IP et port du serveur

        // Création et connexion du socket
        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connecté au serveur !" << std::endl;

        // Envoi de son id
        boost::asio::write(socket, boost::asio::buffer(your_id+"\n"));

        // Lecture de la réponse du serveur
        char reply[1024];
        size_t reply_length = socket.read_some(boost::asio::buffer(reply, 1024));
        std::cout << "Réponse du serveur : ";
        std::cout.write(reply, reply_length);
        std::cout << std::endl;
        std::string response(reply,reply_length);
        if(response == "OK\n")
        {
            std::vector<unsigned char> pdf_data = get_pdf_data();
            std::string send_msg(pdf_data.begin(),pdf_data.end());
            std::cout << send_msg << std::endl;
            boost::asio::write(socket, boost::asio::buffer(send_msg));   
            size_t reply_length_ = socket.read_some(boost::asio::buffer(reply, 1024));
            std::cout.write(reply,reply_length_);
            std::cout << "\n";
        }
        else
        {
            std::cerr << "Le serveur n'a pas accepté votre connexion" << std::endl;
            return 1;
        }

    } catch (std::exception& e) {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
}
