#pragma once

#include "core/timing.h"

struct PlanningTimingParts {
    Timing grow;
    Timing carry;
    Timing cull;
};

struct AppTimingParts {
    Timing draw;
    Timing total;
};

struct DurationParts {
    float grow;
    float carry;
    float cull;
    float draw;
    float total;
};
