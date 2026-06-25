#include "Log.h"
#include "Config.h"
#include "PassengerDriver.h"

#include "mod/amlmod.h"

// Informações oficiais do plugin para o AML.
// Esses dados são usados para o mod aparecer na lista de mods carregados.
MYMOD(
    net.lujim.passengerdriver,
    LuJim Passenger Driver,
    1.0.3,
    LuJim Mods
)

// Garante que o AML só tente carregar este plugin no GTA SA Android.
NEEDGAME(com.rockstargames.gtasa)

static bool g_LPDInitialized = false;

static void LPD_SafeInit()
{
    if (g_LPDInitialized) return;
    g_LPDInitialized = true;

    Config::Load();
    LPD_Log("[AML] LuJim Passenger Driver v1.0.3 carregado pelo AML");
    LPD_Log("[AML] Config path: /storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs/LuJimPassengerDriver/LuJimPassengerDriver.ini");

    // Inicialização segura: não instala hooks perigosos.
    // A lógica experimental só roda quando ExperimentalHooks=1 no INI.
    PassengerDriver::Init();
}

ON_MOD_PRELOAD()
{
    // Não inicializar lógica aqui. Alguns sistemas do jogo ainda não estão prontos.
}

ON_MOD_LOAD()
{
    // Inicialização principal do plugin.
    LPD_SafeInit();
}

ON_ALL_MODS_LOAD()
{
    // Segunda chance de inicialização caso o AML carregue interfaces depois.
    LPD_SafeInit();
    LPD_Log("[AML] OnAllModsLoaded - LuJim Passenger Driver pronto");
}

ON_MOD_UNLOAD()
{
    LPD_Log("[AML] OnModUnload - LuJim Passenger Driver");
}
