#pragma once

#include <raylib.h>

struct MomentaryButton {
    Rectangle rec;
    float moment_duration;
    bool engaged = false;
    float last = GetTime();

    bool CheckCollisionPoint(const Vector2 pos) const {
        return CheckCollisionPointRec(pos, rec);
    }

    void update(const bool pressed) {
        const float now = GetTime();
        const float delta = now - last;

        if (pressed) {
            engaged = true;
            last = now;  // reset lockout timer when pressed
        } else {
            if (engaged && (delta >= moment_duration)) {
                engaged = false;  // release after lockout duration
            }
        }
    }
};
