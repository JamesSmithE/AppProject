#pragma once

#include <expected>
#include <cmath>
#include <ctime>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

namespace Protocol {

class TcpConnection
    : public boost::enable_shared_from_this<TcpConnection>
{
public:
    typedef boost::shared_ptr<TcpConnection> pointer;

    TcpConnection(const TcpConnection&) = delete;
    TcpConnection(TcpConnection&&) = delete;

    TcpConnection operator=(const TcpConnection&) = delete;
    TcpConnection operator=(TcpConnection&&) = delete;

    static pointer create(boost::asio::io_context& io_context)
    {
        return pointer(new TcpConnection(io_context));
    }

    tcp::socket& socket()
    {
      return socket_;
    }

    void start()
    {
        message_ = "";

        boost::asio::async_write(socket_,
                                 boost::asio::buffer(message_),
                                 boost::bind(&TcpConnection::handle_write,
                                 shared_from_this(),
                                 boost::asio::placeholders::error,
                                 boost::asio::placeholders::bytes_transferred));
    }

private:
    TcpConnection(boost::asio::io_context& io_context)
        : socket_(io_context)
    {
    }

    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/)
    {
    }

    boost::asio::ip::tcp::socket socket_;
    std::string message_;
};

}
