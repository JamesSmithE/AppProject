#pragma once

#include "AppConcepts.h"

class EncryptionStrategy {
public:

     [[nodiscard]] std::size_t Send(AppConcepts::BufferT&) { return 0; }
     [[nodiscard]] std::size_t Recv(AppConcepts::BufferT&) { return 0; }

    virtual void Start()  { }

};
