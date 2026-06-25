#pragma once
#include "GameTypes.h"
namespace Recruit {
    CPed* FindRecruitForPlayer(CPed* player, bool firstRecruit, bool nearestRecruit);
    bool IsValidRecruit(CPed* ped);
    bool SendToDriverDoor(CPed* recruit, CVehicle* vehicle, bool realisticEntry);
    bool PutInDriverSeatVisual(CPed* recruit, CVehicle* vehicle);
    void UpdateDriverAnimation(CPed* recruit, CVehicle* vehicle);
}
