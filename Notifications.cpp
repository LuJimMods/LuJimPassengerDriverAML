#include "Notifications.h"
#include "Config.h"
#include "Log.h"
void Notifications::Show(const char* text){ if(!Config::values.showNotifications) return; LPD_Log("[NOTIFY] %s", text); /* V1 experimental: log/AML. Mensagem nativa GTA será ligada no próximo hook de CHud. */ }
void Notifications::Enabled(){ Show("~g~LuJim Passenger Driver~n~~w~Modo Passageiro Motorista Ativado"); }
void Notifications::Disabled(){ Show("~r~LuJim Passenger Driver~n~~w~Modo Passageiro Motorista Desativado"); }
void Notifications::NoRecruit(){ Show("~y~LuJim Passenger Driver~n~~w~Nenhum recruta encontrado."); }
void Notifications::Info(const char* text){ Show(text); }
