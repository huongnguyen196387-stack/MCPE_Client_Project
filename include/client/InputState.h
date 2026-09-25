#pragma once

#include <cstdint>

struct InputState {
    bool leftMouseDown{false};
    bool rightMouseDown{false};

    bool keyW{false};
    bool keyA{false};
    bool keyS{false};
    bool keyD{false};

    bool dpadUp{false};
    bool dpadLeft{false};
    bool dpadDown{false};
    bool dpadRight{false};

    std::uint64_t nowMs{0};
};
