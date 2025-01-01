#pragma once

#include "AppConcepts.h"

struct Data {

    Data() = default;
    Data(const Data&) = default;
    Data(Data&&) = default;

    Data operator=(const Data&) = delete;
    Data operator=(Data&&) = delete;

    ~Data() {}

    [[nodiscard]] virtual std::size_t serialize(AppConcepts::BufferT&) { return 0; }

};
