#include "GameSymbols.h"
#include "Log.h"

#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <dlfcn.h>

GameSymbols gSymbols;

namespace
{
    static uintptr_t ReadLibBase(const char* name)
    {
        FILE* f = fopen("/proc/self/maps", "r");
        if(!f) return 0;

        char line[512];
        while(fgets(line, sizeof(line), f))
        {
            if(strstr(line, name))
            {
                uintptr_t start = 0;
                sscanf(line, "%" SCNxPTR "-", &start);
                fclose(f);
                return start;
            }
        }

        fclose(f);
        return 0;
    }

    static uintptr_t ResolveByDlsym(void* handle, const char* symbol)
    {
        if(!symbol) return 0;

        void* p = nullptr;

        if(handle)
        {
            p = dlsym(handle, symbol);
        }

        if(!p)
        {
            p = dlsym(RTLD_DEFAULT, symbol);
        }

        return reinterpret_cast<uintptr_t>(p);
    }

    static uintptr_t ResolveByOffset(uintptr_t base, uintptr_t offset)
    {
        if(!base || !offset) return 0;
        return base + offset;
    }

    static void ResolvePlayerSymbols()
    {
        void* gtasa = dlopen("libGTASA.so", RTLD_NOW | RTLD_NOLOAD);
        if(!gtasa)
        {
            gtasa = dlopen("libGTASA.so", RTLD_NOW);
        }

        gSymbols.findPlayerPed = ResolveByDlsym(gtasa, "_Z13FindPlayerPedi");
        gSymbols.findPlayerVehicle = ResolveByDlsym(gtasa, "_Z17FindPlayerVehicleib");
        gSymbols.vehicleIsDriver = ResolveByDlsym(gtasa, "_ZNK8CVehicle8IsDriverEPK4CPed");

#if defined(__aarch64__)
        // GTA SA Android arm64 v2.00 / DE base offsets conhecidos.
        constexpr uintptr_t kFindPlayerPedOffset = 0x004EFAE0;
        constexpr uintptr_t kFindPlayerVehicleOffset = 0x004EFDEC;
        constexpr uintptr_t kVehicleIsDriverOffset = 0x006A8868;
#else
        // GTA SA Android 32 bits v2.00 AML.
        // Os offsets das funcoes Thumb ja possuem bit 1 no proprio simbolo.
        constexpr uintptr_t kFindPlayerPedOffset = 0x0040B289;
        constexpr uintptr_t kFindPlayerVehicleOffset = 0x0040B531;
        constexpr uintptr_t kVehicleIsDriverOffset = 0x00584A43;
#endif

        if(!gSymbols.findPlayerPed)
        {
            gSymbols.findPlayerPed = ResolveByOffset(gSymbols.base, kFindPlayerPedOffset);
        }

        if(!gSymbols.findPlayerVehicle)
        {
            gSymbols.findPlayerVehicle = ResolveByOffset(gSymbols.base, kFindPlayerVehicleOffset);
        }

        if(!gSymbols.vehicleIsDriver)
        {
            gSymbols.vehicleIsDriver = ResolveByOffset(gSymbols.base, kVehicleIsDriverOffset);
        }

        LPD_Log("[SYMBOLS] V2.3 FindPlayerPed=%p FindPlayerVehicle=%p CVehicle::IsDriver=%p",
                reinterpret_cast<void*>(gSymbols.findPlayerPed),
                reinterpret_cast<void*>(gSymbols.findPlayerVehicle),
                reinterpret_cast<void*>(gSymbols.vehicleIsDriver));

        if(gtasa)
        {
            dlclose(gtasa);
        }
    }
}

void InitGameSymbols()
{
    gSymbols.base = ReadLibBase("libGTASA.so");
    LPD_Log("[SYMBOLS] libGTASA base=%p", reinterpret_cast<void*>(gSymbols.base));

    ResolvePlayerSymbols();

    LPD_Log("[SYMBOLS] SafeTest: hooks perigosos desativados por padrão.");
}

void* GTASA(uintptr_t offset)
{
    if(!gSymbols.base)
    {
        InitGameSymbols();
    }

    if(!gSymbols.base || !offset)
    {
        return nullptr;
    }

    return reinterpret_cast<void*>(gSymbols.base + offset);
}

FindPlayerPedFn GetFindPlayerPed()
{
    if(!gSymbols.findPlayerPed)
    {
        InitGameSymbols();
    }

    return reinterpret_cast<FindPlayerPedFn>(gSymbols.findPlayerPed);
}

FindPlayerVehicleFn GetFindPlayerVehicle()
{
    if(!gSymbols.findPlayerVehicle)
    {
        InitGameSymbols();
    }

    return reinterpret_cast<FindPlayerVehicleFn>(gSymbols.findPlayerVehicle);
}

VehicleIsDriverFn GetVehicleIsDriver()
{
    if(!gSymbols.vehicleIsDriver)
    {
        InitGameSymbols();
    }

    return reinterpret_cast<VehicleIsDriverFn>(gSymbols.vehicleIsDriver);
}
