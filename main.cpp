#include "Config.h"
#include "Log.h"

#include <cstdio>
#include <cstring>

// LuJim Passenger Driver - AML Safe Loader Test
// Esta versao foi feita apenas para confirmar que a lib carrega no AML sem crash.
// Nao inicia hooks, nao inicia thread e nao chama funcoes internas do GTA SA.

struct ModVersionCompat {
    unsigned short major;
    unsigned short minor;
    unsigned short revision;
    unsigned short build;
};

struct ModInfoCompat {
    char szGUID[48];
    char szName[48];
    char szVersion[24];
    char szAuthor[48];
    ModVersionCompat version;
};

static ModInfoCompat gModInfo;
static bool gModInfoReady = false;
static bool gLoaded = false;

static void LPD_Copy(char* dst, size_t size, const char* src)
{
    if (!dst || size == 0) return;
    std::snprintf(dst, size, "%s", src ? src : "");
}

static void PrepareModInfo()
{
    if (gModInfoReady) return;

    std::memset(&gModInfo, 0, sizeof(gModInfo));

    LPD_Copy(gModInfo.szGUID, sizeof(gModInfo.szGUID), "lujim.passengerdriver");
    LPD_Copy(gModInfo.szName, sizeof(gModInfo.szName), "LuJim Passenger Driver");
    LPD_Copy(gModInfo.szVersion, sizeof(gModInfo.szVersion), "1.0.1-safe");
    LPD_Copy(gModInfo.szAuthor, sizeof(gModInfo.szAuthor), "LuJim Mods");

    gModInfo.version.major = 1;
    gModInfo.version.minor = 0;
    gModInfo.version.revision = 1;
    gModInfo.version.build = 0;

    gModInfoReady = true;
}

extern "C" __attribute__((visibility("default"))) void* __GetModInfo()
{
    PrepareModInfo();
    return &gModInfo;
}

extern "C" __attribute__((visibility("default"))) const char* __INeedASpecificGame()
{
    return "com.rockstargames.gtasa";
}

extern "C" __attribute__((visibility("default"))) void OnModPreLoad()
{
    PrepareModInfo();
}

extern "C" __attribute__((visibility("default"))) void OnModLoad()
{
    PrepareModInfo();

    if (gLoaded) return;
    gLoaded = true;

    Config::EnsureDefault();
    Config::Load();

    LPD_Log("[AML] LuJim Passenger Driver carregado com sucesso.");
    LPD_Log("[AML] Versao: 1.0.1-safe");
    LPD_Log("[AML] Caminho do INI: %s", Config::Path());
    LPD_Log("[AML] Hooks desativados nesta versao segura.");
}

extern "C" __attribute__((visibility("default"))) void OnAllModsLoaded()
{
    LPD_Log("[AML] OnAllModsLoaded chamado - LuJim Passenger Driver.");
}
