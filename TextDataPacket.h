#pragma once

#include "DataPacket.h"
#include "TextData.h"

struct TextDataPacket : public DataPacket<TextData>
{
public:
    TextDataPacket(DataPacket<TextData>::DataT&& d)
        : DataPacket<TextData>(std::move(d)) { }

};
