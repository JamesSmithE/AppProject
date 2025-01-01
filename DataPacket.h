#pragma once

#include <string>
#include "Data.h"
#include "AppConcepts.h"

template <AppConcepts::HasSerialize T = Data>
class DataPacket {
public:
    using DataT = T;
    DataPacket(DataT&& d) : data{d} {
    }
    DataPacket(const DataPacket&) {}
    DataPacket(DataPacket&&) = delete;

    virtual ~DataPacket() {}

    DataPacket operator=(const DataPacket&) = delete;
    DataPacket operator=(DataPacket&&) = delete;

private:
    std::string header;
    std::size_t hash;
    std::size_t checksum;
    DataT data;
};
