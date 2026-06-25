#include "Vehicle.h"
#include "GameSymbols.h"
#include "Log.h"
bool Vehicle::IsAllowed(void* vehicle){ return vehicle!=nullptr; }
bool Vehicle::DriverSeatFree(void* vehicle){ return vehicle!=nullptr; }
bool Vehicle::PutPedInCarDirect(void* ped, void* vehicle, int seat){ LPD_Log("[VEHICLE] SetPedInCarDirect ped=%p vehicle=%p seat=%d", ped, vehicle, seat); if(!ped||!vehicle) return false; typedef void (*Fn)(void*,void*,int,bool); auto fn=(Fn)GTASA(gSymbols.CCarEnterExit_SetPedInCarDirect); if(!fn) return false; /* bloqueado por segurança até confirmar assinatura no teste */ return false; }
