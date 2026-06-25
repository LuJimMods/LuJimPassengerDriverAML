#pragma once
#include <cstdint>
struct GameSymbols {
    uintptr_t base = 0;
    // Offsets conhecidos ficam centralizados aqui.
    uintptr_t chudSetHelpMessage = 0;
    uintptr_t cpadUpdate = 0;
    uintptr_t cpedEnterCar = 0;
    uintptr_t ctaskEnterCar = 0;
};
extern GameSymbols gSymbols;
void InitGameSymbols();
void* GTASA(uintptr_t offset);
