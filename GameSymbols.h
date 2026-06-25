#pragma once

#include <cstdint>

struct GameSymbols
{
    uintptr_t base = 0;

    // Funcoes usadas pelo monitor seguro do Passenger Driver.
    uintptr_t findPlayerPed = 0;
    uintptr_t findPlayerVehicle = 0;
    uintptr_t vehicleIsDriver = 0;

    // Offsets reservados para etapas futuras.
    uintptr_t chudSetHelpMessage = 0;
    uintptr_t cpadUpdate = 0;
    uintptr_t cpedEnterCar = 0;
    uintptr_t ctaskEnterCar = 0;
};

extern GameSymbols gSymbols;

void InitGameSymbols();
void* GTASA(uintptr_t offset);

using FindPlayerPedFn = void* (*)(int playerId);
using FindPlayerVehicleFn = void* (*)(int playerId, bool includeRemote);
using VehicleIsDriverFn = bool (*)(void* vehicle, const void* ped);

FindPlayerPedFn GetFindPlayerPed();
FindPlayerVehicleFn GetFindPlayerVehicle();
VehicleIsDriverFn GetVehicleIsDriver();
