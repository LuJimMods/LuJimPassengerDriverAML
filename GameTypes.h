#pragma once
#include <cstdint>

struct CPed;
struct CVehicle;
struct CVector { float x, y, z; };

struct GameState {
    CPed* player = nullptr;
    CVehicle* targetVehicle = nullptr;
    CVehicle* currentVehicle = nullptr;
    CPed* activeRecruit = nullptr;
};
