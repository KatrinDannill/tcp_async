#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <thread>

using boost::asio::ip::tcp;

enum { max_length = 1024 };
void client(const char* host, const char* port, int write_read) {
    try
    {
        boost::asio::io_context io_context;
        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(s, resolver.resolve(host, port));
        for (int i = 0; i < write_read; i++)
        {
            std::cout << "Client " << std::this_thread::get_id() << " -Enter message: " << std::endl;
            char request[max_length];
            std::cin.getline(request, max_length);
            size_t request_size = std::strlen(request);
            boost::asio::write(s, boost::asio::buffer(request, request_size));
            char reply[max_length];
            size_t reply_size = boost::asio::read(s,boost::asio::buffer(reply, request_size));
            std::cout << " Reply is: ";
            std::cout.write(reply, reply_size);
            std::cout << "\n";
        }

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 5)
        {
            std::cerr << "Usage: Client <host> <port> <num_of_clients> <num_of_cicles>\n";
            return 1;
        }
        std::vector<std::thread> threads;
        for (int i = 0; i < std::atoi(argv[3]); i++) {
            threads.push_back(std::thread(client, argv[1], argv[2], std::atoi(argv[4])));
        };

        for (auto& thread : threads) {
            thread.join();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}