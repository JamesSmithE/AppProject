#pragma once

#include "Data.h"

struct TextData : public Data {
    [[nodiscard]] virtual std::size_t serialize(AppConcepts::BufferT&) override { return 0; }
};
