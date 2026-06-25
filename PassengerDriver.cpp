#include "PassengerDriver.h"
#include "Config.h"
#include "Input.h"
#include "UI.h"
#include "Recruit.h"
#include "Vehicle.h"
#include "Log.h"

static ModConfig gCfg;
static bool gRuntimeEnabled = false;

int LPD_GetHoldTimeMs() { return gCfg.holdTimeMs; }

namespace PassengerDriver {
void Init() {
    Config::EnsureDefault();
    gCfg = Config::Load();
    gRuntimeEnabled = gCfg.enabled;
    LPD_LOGI("LuJim Passenger Driver initialized. Enabled=%d Config=%s", gRuntimeEnabled ? 1 : 0, Config::Path());
}

bool IsEnabled() { return gRuntimeEnabled; }

void ToggleEnabled() {
    gRuntimeEnabled = !gRuntimeEnabled;
    gCfg.enabled = gRuntimeEnabled;
    Config::Save(gCfg);
    UI::ShowMessage(gRuntimeEnabled ? "LuJim Passenger Driver Activated" : "LuJim Passenger Driver Disabled");
    LPD_LOGI("Toggled Enabled=%d", gRuntimeEnabled ? 1 : 0);
}

void Update(float dtMs) {
    Input::Update(dtMs);
    if(gCfg.showProgressBar) UI::DrawProgress(Input::HoldProgress());
    if(Input::ConsumeToggleRequest()) ToggleEnabled();

    if(!gRuntimeEnabled) return;

    // TODO: obter player/veículo alvo via símbolos/offsets do GTA SA Android 2.00.
    // Fluxo final:
    // 1) toque rápido no entrar => interceptar
    // 2) encontrar recruta
    // 3) recruta vai à porta motorista
    // 4) player entra no passageiro
    // 5) redirecionar controles do player para o veículo
}
}
