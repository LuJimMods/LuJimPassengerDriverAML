#include "Vehicle.h"
#include "Log.h"
namespace Vehicle {
bool IsAllowed(CVehicle* vehicle, const ModConfig& cfg) {
    (void)vehicle;
    return cfg.allowCars || cfg.allowPolice || cfg.allowCustom19001_19200;
}
bool ForcePlayerPassengerSeat(CPed* player, CVehicle* vehicle) {
    (void)player; (void)vehicle;
    // TODO: hook em task enter car para escolher assento passageiro.
    LPD_LOGI("Player requested to enter passenger seat");
    return player && vehicle;
}
bool RedirectControlsFromPassenger(CPed* player, CVehicle* vehicle) {
    (void)player; (void)vehicle;
    // TODO: aplicar input do player ao CVehicle mesmo sem estar no driver seat.
    return player && vehicle;
}
void CancelPassengerDriver(CPed* player, CVehicle* vehicle) {
    (void)player; (void)vehicle;
    LPD_LOGI("Passenger Driver cancelled");
}
}
