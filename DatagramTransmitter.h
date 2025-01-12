#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <ifaddrs.h>
#include <unistd.h>

#include <functional>
#include <string>

class DatagramTransmitter {
public:
    enum class Error { SOCKET_ERROR, REUSE, BIND, JOIN_GROUP };

    using ErrorHandler = std::function<void(std::string,Error)>;
    using ReceiverReady = std::function<void()>;

    DatagramTransmitter(const std::string bindAddress,
                        const unsigned int portNum,
                        const std::string& multicastAddr,
                        ErrorHandler&& error)
        : bind_address{},
          multicast_address{},
          mreq{},
          onError{std::move(error)} {

    }

    ErrorHandler onError;

    void Start(){
        sendMulticastMessage();
    }

private:
    void sendMulticastFromIp(std::string localIpAddr) {
        struct sockaddr_in local_interface;
        struct sockaddr_in group_socket_address;
        int sd;
        u_int yes = 1;
        char search_message[1024] = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 3\r\nST: urn:schemas-upnp-org:device:MediaRenderer:1\r\n\r\n";
        int datalen = sizeof(search_message);
        int ttl = 4;

        sd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sd < 0)
            return;

        setsockopt(sd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

        memset((char *) &group_socket_address, 0, sizeof(group_socket_address));
        group_socket_address.sin_family = AF_INET;
        group_socket_address.sin_addr.s_addr = inet_addr("239.255.255.250");
        group_socket_address.sin_port = htons(1900);

        char loopch = 0;
        if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0) {
            close(sd);
            return;
        }

        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
            close(sd);
            return;
        }

        memset((char *) &local_interface, 0, sizeof(local_interface));
        local_interface.sin_family = AF_INET;

        local_interface.sin_addr.s_addr = inet_addr(localIpAddr.c_str());

        local_interface.sin_port = htons(1901);

        int err;
        if ((err = bind(sd, (struct sockaddr*)&local_interface, sizeof(local_interface))) < 0) {
            close(sd);
            return;
        }

        if(sendto(sd, search_message, datalen, 0, (struct sockaddr*)&group_socket_address, sizeof(group_socket_address)) < 0) {
            close(sd);
            return;
        }
        close(sd);
    }
    void sendMulticastMessage()
    {
        struct ifaddrs * interface_addresses = NULL;
        struct ifaddrs * interface_address = NULL;
        void * temp_pointer = NULL;

        getifaddrs(&interface_addresses);

        for (interface_address = interface_addresses; interface_address != NULL; interface_address = interface_address->ifa_next) {
            if (!interface_address->ifa_addr || interface_address->ifa_addr->sa_data[2] == 127) {
                continue;
            }
            if (interface_address->ifa_addr->sa_family == AF_INET) { // only send from IPv4 address
                temp_pointer=&((struct sockaddr_in *)interface_address->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, temp_pointer, addressBuffer, INET_ADDRSTRLEN);
                sendMulticastFromIp(addressBuffer);
            }
        }
        if (interface_addresses != NULL)
            freeifaddrs(interface_addresses);

        return;
    }
    int recvSocket;
    const u_int yes = 1;
    const int ttl = 4;
    static constexpr int MaximumResponseLength = 1024 * 1024 * 4;
    struct sockaddr_in bind_address;
    struct in_addr multicast_address;
    struct ip_mreq mreq;
};
