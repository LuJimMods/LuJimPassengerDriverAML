#include <mod/amlmod.h>
#include <mod/logger.h>

#include "Config.h"
#include "Log.h"

// LuJim Passenger Driver - AML Loader Fix
// Esta versão usa os headers reais do AML para o plugin aparecer corretamente
// na lista de mods carregados. Não instala hooks, não inicia threads e não chama
// funções internas do GTA SA. O objetivo é confirmar carregamento estável.

MYMOD(lujim.passengerdriver, LuJim Passenger Driver, 1.0.2, LuJim Mods)
NEEDGAME(com.rockstargames.gtasa)

static bool gLoaded = false;

ON_MOD_PRELOAD()
{
    // Não chama Config nem funções do jogo aqui.
    // Este callback pode rodar cedo demais para mexer em arquivos ou símbolos.
}

ON_MOD_LOAD()
{
    if (gLoaded) return;
    gLoaded = true;

    if (logger)
    {
        logger->SetTag("LuJimPassengerDriver");
        logger->Info("LuJim Passenger Driver: OnModLoad chamado.");
    }

    Config::EnsureDefault();
    Config::Load();

    LPD_Log("[AML] LuJim Passenger Driver carregado com sucesso.");
    LPD_Log("[AML] Versao: 1.0.2-loaderfix");
    LPD_Log("[AML] Caminho do INI: %s", Config::Path());
    LPD_Log("[AML] Caminho do LOG: /storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs/LuJimPassengerDriver/LuJimPassengerDriver.log");
    LPD_Log("[AML] Hooks desativados nesta versao de teste.");
}

ON_ALL_MODS_LOAD()
{
    if (logger)
    {
        logger->Info("LuJim Passenger Driver: OnAllModsLoaded chamado.");
    }

    LPD_Log("[AML] OnAllModsLoaded chamado - LuJim Passenger Driver.");
}
