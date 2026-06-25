#include "GameSymbols.h"
#include "Log.h"

#include <cstdio>
#include <cstring>
#include <cinttypes>

GameSymbols gSymbols;

namespace
{
    static uintptr_t ReadLibBase(const char* name)
    {
        FILE* f = fopen("/proc/self/maps", "r");
        if(!f)
        {
            LPD_Log("[SYMBOLS] ERRO: nao foi possivel abrir /proc/self/maps");
            return 0;
        }

        char line[512];
        while(fgets(line, sizeof(line), f))
        {
            if(strstr(line, name))
            {
                uintptr_t start = 0;
                if(sscanf(line, "%" SCNxPTR "-", &start) == 1)
                {
                    fclose(f);
                    return start;
                }
            }
        }

        fclose(f);
        return 0;
    }

    static uintptr_t ResolveByOffset(uintptr_t base, uintptr_t offset)
    {
        if(!base || !offset)
        {
            return 0;
        }
        return base + offset;
    }

    static void ResolvePlayerSymbols()
    {
        if(!gSymbols.base)
        {
            LPD_Log("[SYMBOLS] ERRO: base da libGTASA invalida. Nao e possivel resolver simbolos.");
            return;
        }

#if defined(__aarch64__)
        // libGTASA.so arm64-v8a.
        // Confirmado via tabela de simbolos:
        // _Z13FindPlayerPedi                  0x004EFAE0
        // _Z17FindPlayerVehicleib            0x004EFDEC
        // _ZNK8CVehicle8IsDriverEPK4CPed     0x006A8868
        constexpr uintptr_t kFindPlayerPedOffset = 0x004EFAE0;
        constexpr uintptr_t kFindPlayerVehicleOffset = 0x004EFDEC;
        constexpr uintptr_t kVehicleIsDriverOffset = 0x006A8868;
#else
        // libGTASA.so armeabi-v7a / GTA SA Android 2.00 AML.
        // Esses offsets ja estao com o bit Thumb quando necessario.
        // Confirmado via tabela de simbolos:
        // _Z13FindPlayerPedi                  0x0040B289
        // _Z17FindPlayerVehicleib            0x0040B531
        // _ZNK8CVehicle8IsDriverEPK4CPed     0x00584A43
        constexpr uintptr_t kFindPlayerPedOffset = 0x0040B289;
        constexpr uintptr_t kFindPlayerVehicleOffset = 0x0040B531;
        constexpr uintptr_t kVehicleIsDriverOffset = 0x00584A43;
#endif

        gSymbols.findPlayerPed = ResolveByOffset(gSymbols.base, kFindPlayerPedOffset);
        gSymbols.findPlayerVehicle = ResolveByOffset(gSymbols.base, kFindPlayerVehicleOffset);
        gSymbols.vehicleIsDriver = ResolveByOffset(gSymbols.base, kVehicleIsDriverOffset);

        LPD_Log("[SYMBOLS] V2.5.0 offsets diretos aplicados");
        LPD_Log("[SYMBOLS] FindPlayerPed=%p offset=0x%" PRIxPTR,
                reinterpret_cast<void*>(gSymbols.findPlayerPed),
                kFindPlayerPedOffset);
        LPD_Log("[SYMBOLS] FindPlayerVehicle=%p offset=0x%" PRIxPTR,
                reinterpret_cast<void*>(gSymbols.findPlayerVehicle),
                kFindPlayerVehicleOffset);
        LPD_Log("[SYMBOLS] CVehicle::IsDriver=%p offset=0x%" PRIxPTR,
                reinterpret_cast<void*>(gSymbols.vehicleIsDriver),
                kVehicleIsDriverOffset);

        if(gSymbols.findPlayerPed && gSymbols.findPlayerVehicle && gSymbols.vehicleIsDriver)
        {
            LPD_Log("[SYMBOLS] V2.5.0 simbolos do jogador/veiculo resolvidos com sucesso.");
        }
        else
        {
            LPD_Log("[SYMBOLS] AVISO: um ou mais simbolos continuam invalidos.");
        }
    }
}

void InitGameSymbols()
{
    gSymbols = GameSymbols{};
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
