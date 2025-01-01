#pragma once

#include <expected>
#include <cmath>
#include <ctime>
#include <boost/asio.hpp>
#include "TcpConnection.h"

using namespace std;
using boost::asio::ip::tcp;

namespace Protocol {

class TcpHost
{
public:
    TcpHost(boost::asio::io_context& io_context)
        : io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
    {
        startAccept();
    }

    TcpHost(const TcpHost&) = delete;
    TcpHost(TcpHost&&) = delete;

    TcpHost operator=(const TcpHost&) = delete;
    TcpHost operator=(TcpHost&&) = delete;


private:
    void startAccept()
    { /*
        TcpConnection::pointer newConnection = TcpConnection::create(io_context_);
        acceptor_.async_accept(newConnection->socket(),
                               std::bind(&TcpHost::handleAccept,
                                         this,
                                         newConnection,
                                         boost::asio::placeholders::error));
      */
    }

    void handleAccept(TcpConnection::pointer newConnection,
                      const boost::system::error_code& error)
    {
        if (!error)
        {
            newConnection->start();
        }

        startAccept();
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

}
