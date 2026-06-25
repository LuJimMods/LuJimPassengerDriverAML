#include "Notifications.h"
#include "Config.h"
#include "Log.h"
#include <cstdio>
void Notifications::Show(const char* text){
    if(!LPDSettings::values.showNotifications) return;
    // Build seguro: grava no log. O desenho real no HUD será ligado quando o hook de CHud estiver confirmado.
    LPD_Log("[NOTIFICACAO] %s", text);
}
void Notifications::Enabled(){ Show("~g~LuJim Passenger Driver~n~~w~Modo Passageiro Motorista Ativado"); }
void Notifications::Disabled(){ Show("~r~LuJim Passenger Driver~n~~w~Modo Passageiro Motorista Desativado"); }
void Notifications::NoRecruit(){ Show("~y~LuJim Passenger Driver~n~~w~Nenhum recruta encontrado."); }
void Notifications::Progress(float percent, bool enabling){
    if(!LPDSettings::values.showProgressBar) return;
    char buf[160];
    std::snprintf(buf, sizeof(buf), "~b~LuJim Passenger Driver~n~~w~%s %.0f%%", enabling ? "Ativando..." : "Desativando...", percent * 100.0f);
    Show(buf);
}
