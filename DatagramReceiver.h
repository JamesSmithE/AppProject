#pragma once


#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>


#include <unistd.h>
#include <sys/select.h>
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

class DatagramReceiver {
public:
    enum class Error { SOCKET_ERROR, REUSE, BIND, JOIN_GROUP };

    using ErrorHandler = std::function<void(std::string,Error)>;
    using ReceiverReady = std::function<void()>;

    DatagramReceiver(const std::string bindAddress,
                  const unsigned int portNum,
                  const std::string& multicastAddr,
                  ErrorHandler error, int)

        : bind_address{}, multicast_address{}, mreq{}, onError{std::move(error)}  {

        memset((char *) &bind_address, 0, sizeof(bind_address));
        bind_address.sin_family = AF_INET;
        bind_address.sin_addr.s_addr = inet_addr(bindAddress.c_str());
        bind_address.sin_port = htons(portNum);

        multicast_address.s_addr = inet_addr(multicastAddr.c_str());
        mreq.imr_multiaddr = multicast_address;
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);

        if ((recvSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            onError("Error occured while creating socket", Error::SOCKET_ERROR);
        } else {
            setsockopt(recvSocket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
        }
    }

    ~DatagramReceiver() {
         setsockopt(recvSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
         close(recvSocket);
    }

    DatagramReceiver(const DatagramReceiver&) = delete;
    DatagramReceiver(DatagramReceiver&&) = delete;

    DatagramReceiver operator=(const DatagramReceiver&) = delete;
    DatagramReceiver operator=(DatagramReceiver&&) = delete;

    void listen() {

        if (setsockopt(recvSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
            close(recvSocket);
            onError("error: can't reuse address.", Error::REUSE);
            return;
        }

        if (bind(recvSocket, (struct sockaddr*)&bind_address, sizeof(bind_address)) < 0) {
            close(recvSocket);
            onError("error: can't bind address.", Error::BIND);
            return;
        }

        if (setsockopt(recvSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
            close(recvSocket);
            onError("error: can't join group.", Error::JOIN_GROUP);
            return;
        }
        readyToReceive();
    }

    std::string getResponses() {

        std::string response_string = "";
        std::vector<char> buffer;
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(recvSocket, &fds);

        timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;

        auto totalBytes = 0;
        int reason = 0;

        while ((reason = select(recvSocket + 1, &fds, NULL, NULL, &tv)) > 0) {

            buffer.resize(MAXLEN + 1);

            auto result = recv(recvSocket, buffer.data(), MAXLEN, 0);

            if(result > 0) {
                totalBytes += result;
                if(totalBytes >= MaximumResponseLength)
                    break;
                try {
                    response_string.append(buffer.data(), result);
                } catch(...) {

                }
            }
        }
        return response_string;
    }

    void startListening()    {

    }

private:
    int recvSocket;
    const u_int yes = 1;
    const int ttl = 4;
    static constexpr int MaximumResponseLength = 1024 * 1024 * 4;
    struct sockaddr_in bind_address;
    struct in_addr multicast_address;
    struct ip_mreq mreq;
    ErrorHandler onError;
    ReceiverReady readyToReceive;
};
