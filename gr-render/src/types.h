#pragma once

#include <cstdint>

namespace grr {
    // 1 byte unsigned - 0 to 255
    using u8 = std::uint8_t;
    
    // 2 byte unsigned - 0 to 65.535
    using u16 = std::uint16_t;
    
    // 4 byte unsigned - 0 to 4.294.967.295
    using u32 = std::uint32_t;
}
