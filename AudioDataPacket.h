#pragma once

#include "AudioData.h"
#include "DataPacket.h"

struct AudioDataPacket : public DataPacket<AudioData> {};
