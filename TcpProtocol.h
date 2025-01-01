#pragma once

#include <expected>
#include <cmath>
#include <ctime>
#include <memory>
#include <boost/asio.hpp>

#include "TcpConnection.h"
#include "TcpHost.h"

using namespace std;
using boost::asio::ip::tcp;

namespace Protocol {

class TcpProtocol {
public:

private:
    std::shared_ptr<TcpConnection> tcpConnection;
    std::shared_ptr<TcpHost> tcpHost;
};

}
