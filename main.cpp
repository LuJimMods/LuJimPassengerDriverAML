#include <mod/amlmod.h>

#include "Log.h"
#include "Config.h"
#include "GameSymbols.h"
#include "PassengerDriver.h"

// Registro AML limpo, sem depender de logger.cpp/config.cpp do AML.
// Mantem a interface oficial do AML, mas usa apenas o log proprio do LuJim Passenger Driver.
MYMOD(net.lujim.passengerdriver, LuJimPassengerDriver, 2.0.0, LuJimMods)
NEEDGAME(com.rockstargames.gtasa)

static bool g_initialized = false;
static bool g_allModsLoaded = false;

ON_MOD_PRELOAD()
{
    LPD_Log("[AML] OnModPreLoad - LuJim Passenger Driver V2.0.0");
}

ON_MOD_LOAD()
{
    LPD_Log("[AML] OnModLoad - LuJim Passenger Driver V2.0.0 chamado");

    if(g_initialized)
    {
        LPD_Log("[AML] OnModLoad ignorado: mod ja inicializado");
        return;
    }

    g_initialized = true;

    // Inicializacao segura: cria/carrega INI antes de qualquer logica do motorista.
    LPDSettings::EnsureDefault();
    LPDSettings::Load();

    // Mantido, mas as rotinas perigosas continuam controladas pelo INI do projeto.
    InitGameSymbols();
    PassengerDriver::Init();

    LPD_Log("[AML] Inicializacao concluida - LuJim Passenger Driver V2.0.0");
}

ON_ALL_MODS_LOAD()
{
    g_allModsLoaded = true;
    LPD_Log("[AML] OnAllModsLoaded - LuJim Passenger Driver V2.0.0");
}
