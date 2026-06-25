#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#include "Log.h"
#include "Config.h"
#include "PassengerDriver.h"
#include "GameSymbols.h"

// Mesmo padrão de registro AML usado pelo GiroflexVSL.
// MYMODCFG cria: modinfo, aml e cfg.
MYMODCFG(net.lujim.passengerdriver, LuJimPassengerDriver, 1.0.4, LuJimMods)
NEEDGAME(com.rockstargames.gtasa)

static bool g_initialized = false;
static bool g_allModsLoaded = false;

extern "C" void OnModPreLoad()
{
    if(logger)
    {
        logger->SetTag("LuJimPassengerDriver");
        logger->Info("[AML] OnModPreLoad chamado");
    }

    LPD_Log("[AML] OnModPreLoad - LuJim Passenger Driver");
}

extern "C" void OnModLoad()
{
    if(logger)
    {
        logger->SetTag("LuJimPassengerDriver");
        logger->Info("[AML] OnModLoad chamado");
    }

    LPD_Log("[AML] OnModLoad - LuJim Passenger Driver carregado");

    if(g_initialized)
    {
        LPD_Log("[AML] OnModLoad ignorado: ja inicializado");
        return;
    }

    g_initialized = true;

    // Primeiro teste seguro: apenas carrega/cria INI e log.
    // Hooks perigosos continuam controlados por ExperimentalHooks no INI.
    LPDSettings::EnsureDefault();
    LPDSettings::Load();

    InitGameSymbols();
    PassengerDriver::Init();

    LPD_Log("[AML] Inicializacao segura concluida");
}

extern "C" void OnAllModsLoaded()
{
    g_allModsLoaded = true;

    if(logger)
    {
        logger->Info("[AML] OnAllModsLoaded chamado");
    }

    LPD_Log("[AML] OnAllModsLoaded - LuJim Passenger Driver");
}
