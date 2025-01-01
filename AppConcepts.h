#pragma once

#include <vector>
#include <string>

namespace AppConcepts {

using BufferT = std::vector<char>;

template <typename DataT>
concept HasSerialize = requires(DataT data, BufferT buf) {
    {data.serialize(buf)} -> std::same_as<std::size_t>;
};

template <typename EncryptStrategy>
concept HasCrypto = requires(EncryptStrategy es, BufferT& buf) {
    {es.Encrypt(buf)} -> std::same_as<std::size_t>;
    {es.Decrypt(buf)} -> std::same_as<std::size_t>;
};

template <typename ProtocolStrategy>
concept HasTransRecv = requires(ProtocolStrategy strategy, BufferT& buf) {
    {strategy.Send(buf)} -> std::same_as<std::size_t>;
    {strategy.Recv(buf)} -> std::same_as<std::size_t>;
};

template <typename ProtocolInterface>
concept HasProtocolSignature = requires (ProtocolInterface pi,
                                 const std::string bindAddress,
                                 const unsigned int portNum,
                                 const std::string& multicastAddr) {
    {pi(bindAddress,portNum,multicastAddr)};
};

template <typename T>
concept DefaultConstructible = requires(T object) {
    { object() };
};

template <typename T>
concept HasMethod = requires(T object) {
    {object.method()} -> std::same_as<void> ;
};

}

