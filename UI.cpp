#include "UI.h"
#include "Log.h"
namespace UI {
void ShowMessage(const char* msg) { LPD_LOGI("MESSAGE: %s", msg ? msg : ""); }
void DrawProgress(float progress) { (void)progress; /* TODO: desenhar barra via HUD/RenderWare hook */ }
}
