#pragma once

#include "core/timing/timing.h"

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

inline DurationParts averageDurations(const PlanningTimingParts &planning, const AppTimingParts &app) {
  return {
      planning.grow.averageDuration(),
      planning.carry.averageDuration(),
      planning.cull.averageDuration(),
      app.draw.averageDuration(),
      app.total.averageDuration(),
  };
}
