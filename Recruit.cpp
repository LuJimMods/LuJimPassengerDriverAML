#include "Recruit.h"
#include "GameSymbols.h"
#include "Log.h"
void* Recruit::FindBestRecruit(void* player){ LPD_Log("[RECRUIT] Busca de recruta chamada. player=%p", player); return nullptr; }
bool Recruit::IsValid(void* ped){ return ped!=nullptr; }
