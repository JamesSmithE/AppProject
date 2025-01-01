#pragma once

#include "AppConcepts.h"
#include "TransportProtocol.h"
#include "EncryptionStrategy.h"

class SecureTransportProtocol
    : public TransportProtocol<EncryptionStrategy> {
public:
    SecureTransportProtocol() {}
    virtual ~SecureTransportProtocol() {}

    virtual void Start() override {
    }

    virtual void Encrypt(AppConcepts::BufferT&) {}
    virtual void Decrypt(AppConcepts::BufferT&) {}
};
