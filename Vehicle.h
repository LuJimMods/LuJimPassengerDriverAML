#pragma once
#include "GameTypes.h"
#include "Config.h"
namespace Vehicle {
    bool IsAllowed(CVehicle* vehicle, const ModConfig& cfg);
    bool ForcePlayerPassengerSeat(CPed* player, CVehicle* vehicle);
    bool RedirectControlsFromPassenger(CPed* player, CVehicle* vehicle);
    void CancelPassengerDriver(CPed* player, CVehicle* vehicle);
}
