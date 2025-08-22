#pragma once

#include <raylib.h>

#include <sstream>
#include <string>

std::string Vector2ToString(const Vector2& v) {
    std::ostringstream ss;
    ss << "(" << v.x << ", " << v.y << ")";
    return ss.str();
}
