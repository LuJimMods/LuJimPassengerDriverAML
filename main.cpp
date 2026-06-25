#include "Log.h"

#include "mod/amlmod.h"

MYMOD(
    net.lujim.passengerdriver,
    LuJim Passenger Driver,
    1.0,
    LuJim Mods
)

extern "C" void OnModPreLoad()
{
    LPD_Log("[AML] OnModPreLoad - LuJim Passenger Driver");
}

extern "C" void OnModLoad()
{
    LPD_Log("[AML] OnModLoad - LuJim Passenger Driver carregado");
}

extern "C" void OnAllModsLoaded()
{
    LPD_Log("[AML] OnAllModsLoaded - LuJim Passenger Driver");
}
