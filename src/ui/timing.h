#pragma once

#include <raylib.h>

#include <deque>
#include <numeric>
#include <utility>

struct Observation {
    float timestamp;
    float duration;
};

struct Timing {
    std::deque<Observation> history;
    float start_time;

    void start() {
        start_time = GetTime();
    }

    void record() {
        const float now = GetTime();
        const float duration = now - start_time;
        history.emplace_back(Observation{now, duration});

        // Remove old entries outside the window.
        while (!history.empty() && ((now - history.front().timestamp) > TIMING_WINDOW_SEC)) {
            history.pop_front();
        }
    }

    float averageDuration() const {
        if (history.empty()) {
            return 0.0f;
        }
        const float sum = std::accumulate(history.begin(), history.end(), 0.0f,
                                          [](float s, const auto& h) { return s + h.duration; });
        return sum / history.size();
    }
};