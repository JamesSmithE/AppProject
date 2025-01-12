#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <atomic>
#include <unistd.h>
#include <sys/select.h>
#include <functional>
#include <string>
#include <memory>
#include <thread>

// #include "AppConcepts.h"
#include "DatagramTransmitter.h"

using ResponseDataType = std::string;

template <typename Protocol = DatagramTransmitter>
class WhoIsTransmitter {
public:




};
