#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/select.h>

#include <functional>
#include <string>
#include <memory>

#include "AppConcepts.h"

#include "DatagramReceiver.h"

#define MAXLEN 2048
#define TIMEOUT_SEC 8

template <AppConcepts::HasProtocolSignature Protocol = DatagramReceiver>
class WhoIsReceiver {
public:
    enum class Error { SOCKET_ERROR, REUSE, BIND, JOIN_GROUP };

    using ErrorHandler  = std::function<void(std::string,Error)>;
    using ReceiverReady = std::function<void()>;

    WhoIsReceiver(const std::string bindAddress,
                  const unsigned int portNum,
                  const std::string& multicastAddr) {

        protocol = std::make_unique<Protocol>(bindAddress,portNum,multicastAddr, [this] (){});
        protocol->readyToReceive = [this]() {
            getResponseData();
        };

        protocol->start();
    }

    WhoIsReceiver(const WhoIsReceiver&) = delete;
    WhoIsReceiver(WhoIsReceiver&&) = delete;

    WhoIsReceiver operator=(const WhoIsReceiver&) = delete;
    WhoIsReceiver operator=(WhoIsReceiver&&) = delete;

    void getResponseData() {
        std::string msg = protocol->getResponses();
    }

private:
    ErrorHandler onError;
    ReceiverReady readyToReceive;
    std::unique_ptr<Protocol> protocol;
};
