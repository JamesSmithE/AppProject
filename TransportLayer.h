#pragma once

#include "TransportStrategy.h"

template <typename TPStrategy = TransportStrategy<>>
class TransportLayer : public TPStrategy {

    void Start() {
    }

private:

};
