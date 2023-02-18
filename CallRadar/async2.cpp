#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class RadarClient
{
public:
    RadarClient(boost::asio::io_context& io_context)
        : io_context_(io_context),
        socket_(io_context)
    {
    }

    void start()
    {
        // Connect to the server asynchronously
        tcp::resolver resolver(io_context_);
        tcp::resolver::query query("192.168.1.199", "5000");
        resolver.async_resolve(query,
            std::bind(&RadarClient::handle_resolve, this,
                std::placeholders::_1, std::placeholders::_2));
    }

private:
    void handle_resolve(const boost::system::error_code& error,
        tcp::resolver::results_type endpoints)
    {
        if (!error)
        {
            // Connect to the server asynchronously
            boost::asio::async_connect(socket_, endpoints,
                std::bind(&RadarClient::handle_connect, this,
                    std::placeholders::_1));
        }
        else
        {
            std::cerr << "Error: " << error.message() << std::endl;
        }
    }

    void handle_connect(const boost::system::error_code& error)
    {
        if (!error)
        {
            std::cout << "Connected to server." << std::endl;

            // Send the start command asynchronously
            std::string start_cmd = "\x73\x62";
            boost::asio::async_write(socket_, boost::asio::buffer(start_cmd),
                std::bind(&RadarClient::handle_write, this,
                    std::placeholders::_1, std::placeholders::_2));
        }
        else
        {
            std::cerr << "Connection error: " << error.message() << std::endl;
        }
    }

    void handle_write(const boost::system::error_code& error,
        std::size_t bytes_transferred)
    {
        if (!error)
        {
            std::cout << "Sent " << bytes_transferred << " bytes to server." << std::endl;
        }
        else
        {
            std::cerr << "Write error: " << error.message() << std::endl;
        }
    }

private:
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
};

{
    try
    {
        boost::asio::io_context io_context;

        // Create a new instance of the radar client
        RadarClient client(io_context);

        // Start the client
        client.start();

        // Run the I/O service to start the asynchronous operations
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
