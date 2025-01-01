#pragma once

#include "TcpProtocol.h"

template <typename NetworkProtocol = Protocol::TcpProtocol>
class TransportStrategy : public NetworkProtocol {
public:

};
