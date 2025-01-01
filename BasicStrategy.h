#pragma once

#include "AppConcepts.h"

class BasicProtocolStrategy {
public:
    [[nodiscard]] std::size_t Send(AppConcepts::BufferT&) { return 0; }
    [[nodiscard]] std::size_t Recv(AppConcepts::BufferT&) { return 0; }
};
