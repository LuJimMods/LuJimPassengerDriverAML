#include "Vehicle.h"
#include "Log.h"
void* Vehicle::FindTargetVehicle(){
    LPD_Log("[VEHICLE] Busca de veículo ainda bloqueada no SafeTest.");
    return nullptr;
}
bool Vehicle::IsAllowed(void*){ return false; }
