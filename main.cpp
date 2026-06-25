// LuJim Passenger Driver AML - experimental base
// Target: GTA SA Android 2.00 AML armeabi-v7a
// Goal: player enters/uses right passenger seat but controls vehicle; NPC stays on left seat as visual driver.
// NOTE: This is a base. The real seat-control part needs GTA SA Mobile addresses/classes from your dump/SDK.

#include <android/log.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define LOG_TAG "PassengerDriverLuJim"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Keep this plugin loadable by AML even before you wire headers.
extern "C" __attribute__((visibility("default"))) void OnModLoad();

// -------------------------
// Minimal GTA placeholders
// Replace with real CEntity/CPed/CVehicle definitions from your SDK/dump.
// -------------------------
struct CVector { float x, y, z; };
struct CPed;
struct CVehicle;

// Offsets must be confirmed in your libGTASA.so 2.00 dump.
namespace Offsets
{
    // These are intentionally 0: fill them from symbols/patterns before enabling hard hooks.
    static uintptr_t g_libGTASA = 0;
    static uintptr_t CWorld_Players = 0;
    static uintptr_t CPlayerPed_ProcessControl = 0;
    static uintptr_t CVehicle_ProcessControl = 0;
    static uintptr_t CCarCtrl_CreateRandomPed = 0;
    static uintptr_t CPed_PutIntoVehicle = 0;
    static uintptr_t CVehicle_SetDriver = 0;
    static uintptr_t CVehicle_SetPassenger = 0;
}

// Runtime config
static bool g_enabled = true;
static bool g_debug = true;
static CPed* g_fakeDriver = nullptr;
static CVehicle* g_controlledVehicle = nullptr;

static void Debug(const char* msg)
{
    if(g_debug) LOGI("%s", msg);
}

// -------------------------
// Functions you need to bind
// -------------------------
using tPutPedIntoVehicle = void(*)(CPed* ped, CVehicle* veh, int seatId);
using tSetDriver = void(*)(CVehicle* veh, CPed* ped);
using tSetPassenger = void(*)(CVehicle* veh, CPed* ped, int seatId);

tPutPedIntoVehicle PutPedIntoVehicle = nullptr;
tSetDriver SetDriver = nullptr;
tSetPassenger SetPassenger = nullptr;

static bool BindGameFunctions()
{
    if(Offsets::g_libGTASA == 0)
    {
        LOGE("libGTASA base not set. Wire this to AML GetLib or dlopen/dladdr.");
        return false;
    }

    if(Offsets::CPed_PutIntoVehicle)
        PutPedIntoVehicle = (tPutPedIntoVehicle)(Offsets::g_libGTASA + Offsets::CPed_PutIntoVehicle);
    if(Offsets::CVehicle_SetDriver)
        SetDriver = (tSetDriver)(Offsets::g_libGTASA + Offsets::CVehicle_SetDriver);
    if(Offsets::CVehicle_SetPassenger)
        SetPassenger = (tSetPassenger)(Offsets::g_libGTASA + Offsets::CVehicle_SetPassenger);

    return true;
}

// -------------------------
// Main idea
// -------------------------
static bool IsPassengerDriverVehicle(CVehicle* veh)
{
    return veh != nullptr && g_enabled && veh == g_controlledVehicle;
}

static void AttachPlayerAsRightPassenger(CPed* player, CVehicle* veh)
{
    if(!player || !veh) return;

    // GTA SA common seat IDs often use 0=front passenger, 1/2 rear seats.
    // Mobile may differ; confirm by testing.
    const int RIGHT_FRONT_PASSENGER = 0;

    if(SetPassenger) SetPassenger(veh, player, RIGHT_FRONT_PASSENGER);
    else if(PutPedIntoVehicle) PutPedIntoVehicle(player, veh, RIGHT_FRONT_PASSENGER);

    g_controlledVehicle = veh;
    Debug("Player moved to right passenger seat.");
}

static void AttachNpcAsVisualDriver(CPed* npc, CVehicle* veh)
{
    if(!npc || !veh) return;

    if(SetDriver) SetDriver(veh, npc);
    else if(PutPedIntoVehicle) PutPedIntoVehicle(npc, veh, -1); // -1 is commonly driver seat in some SA codebases.

    g_fakeDriver = npc;
    Debug("NPC attached as visual driver.");
}

static void ApplyPlayerControlsToVehicle(CPed* player, CVehicle* veh)
{
    if(!player || !veh || !IsPassengerDriverVehicle(veh)) return;

    // This is the part that needs real GTASA structures:
    // - read CPad for player
    // - write vehicle m_fGasPedal / m_fBreakPedal / m_fSteerAngle or call Automate driver input
    // - keep player task as passenger, not driver
    // - keep fake NPC task/anims as driver
    // Example pseudocode:
    // veh->m_fGasPedal = pad->NewState.Accelerate ? 1.0f : 0.0f;
    // veh->m_fBreakPedal = pad->NewState.Brake ? 1.0f : 0.0f;
    // veh->m_fSteerInput = pad->NewState.LeftStickX;
}

// Call this from a hook when player tries to enter a vehicle.
static void StartPassengerDriving(CPed* player, CVehicle* veh, CPed* npcDriver)
{
    if(!g_enabled || !player || !veh) return;

    AttachPlayerAsRightPassenger(player, veh);
    AttachNpcAsVisualDriver(npcDriver, veh);
}

extern "C" void OnModLoad()
{
    LOGI("LuJim Passenger Driver AML loaded.");
    LOGI("Status: base loaded. Wire GTASA offsets/hooks before real gameplay use.");
    BindGameFunctions();
}
