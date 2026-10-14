#pragma once

#include "config.h"
#include "core/ctrl_state.h"

struct PlanSettings {
  int num_carry;
  int num_samples;
  bool rewire_enabled;
};

PlanSettings getPlanSettings(const CtrlState &state) {
  return {NUM_CARRY_OPTIONS[state.num_carry_ix], NUM_SAMPLES_OPTIONS[state.num_samples_ix], state.rewire_enabled};
}
