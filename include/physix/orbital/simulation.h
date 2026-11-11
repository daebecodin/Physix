#pragma once
#include "body.h"
#include <vector>

namespace Physix {

    void simulateSystem(std::vector<Body>& bodies, double dt);
}
