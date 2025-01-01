#pragma once

#include "AppConcepts.h"
#include "BasicStrategy.h"

template <AppConcepts::HasTransRecv Strategy = BasicProtocolStrategy>
class TransportProtocol : public Strategy {
    virtual void Start() {}
};
