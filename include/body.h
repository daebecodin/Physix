#pragma once

#include "vec3.h"
#include <string>

struct Body {
    std::string name;
    double mass {};
    Vec3 position {};
    Vec3 velocity {};
};
