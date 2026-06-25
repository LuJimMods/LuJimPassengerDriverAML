#pragma once
namespace Input {
    void Update(float dtMs);
    bool ConsumeToggleRequest();
    float HoldProgress();
    bool IsEnterPressed();
}
