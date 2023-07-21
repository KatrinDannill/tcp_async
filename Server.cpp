#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <chrono>
#include <boost/asio/steady_timer.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, int& counter)
        : socket_(std::move(socket)), counter_(counter), timer_(socket_.get_executor(), std::chrono::steady_clock::time_point::max())
    {
        ++counter_;
        std::cout << "connection established: " << counter_ << std::endl;
    }

    ~session()
    {
        std::cout << "connection lost: " << counter_ << std::endl;
        --counter_;
    }

    void start()
    {
        do_read();
        do_start_timer();
    }

private:
    void do_read()
    {
        timer_.cancel();
        do_start_timer();
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length)
            {

                if (!ec)
                {
                    do_write(length);
                }
            });
    }

    void do_write(std::size_t length)
    {
        timer_.cancel();
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
            [this, self](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    do_read();
                }
            });
    }

    void do_start_timer()
    {
        timer_.expires_after(std::chrono::seconds(15)); 
        timer_.async_wait(
            [this](const boost::system::error_code& ec)
            {
                if (!ec)
                {
                    socket_.close(); 
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    int& counter_;
    boost::asio::steady_timer timer_;
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
        counter_(0)
    {}
    void do_accept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<session>(std::move(socket), counter_)->start();
                }
       
        do_accept();
            });
    }

    tcp::acceptor acceptor_;
    int counter_;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: Server <port>\n";
            return 1;
        }
        else
        {
            std::cout << "Thats good";
        }
        boost::asio::io_context io_context;
        server s(io_context, std::atoi(argv[1]));
        s.do_accept();
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
