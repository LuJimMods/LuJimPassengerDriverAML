#pragma once
#include <cstdint>
struct GameSymbols {
    uintptr_t base=0;
    uintptr_t CWidgetButtonEnterCar_SetEnterCarVehicleType=0x002b60b1;
    uintptr_t CPad_ExitVehicleJustDown=0x003faa2d;
    uintptr_t CPad_GetBrake=0x003fa95d;
    uintptr_t CPedGroups_GetPedsGroup=0x004b408d;
    uintptr_t CPedGroupMembership_GetLeader=0x004b22ed;
    uintptr_t CPedGroup_GetClosestGroupPed=0x004b7015;
    uintptr_t CTaskComplexEnterCarAsPassengerTimed_Ctor=0x004f760d;
    uintptr_t CTaskComplexEnterCarAsPassengerTimed_CreateFirstSubTask=0x004f7721;
    uintptr_t CTaskComplexEnterAnyCarAsDriver_CreateNextSubTask=0x004fc981;
    uintptr_t CTaskSimpleCarSetPedInAsPassenger_Ctor_D1=0x005026f9;
    uintptr_t CTaskSimpleCarSetPedInAsDriver_D1=0x00502d01;
    uintptr_t CCarEnterExit_GetNearestCarPassengerDoor=0x005074f1;
    uintptr_t CCarEnterExit_SetPedInCarDirect=0x0050aa59;
    uintptr_t CVehicle_RemovePassenger=0x005844d9;
    uintptr_t CVehicle_IsPassenger=0x005849cb;
    uintptr_t CVehicle_PickRandomPassenger=0x0058a76d;
    uintptr_t FindPlayerCentreOfWorldForMap=0x0040bce9;
};
extern GameSymbols gSymbols;
void InitGameSymbols();
void* GTASA(uintptr_t offset);
