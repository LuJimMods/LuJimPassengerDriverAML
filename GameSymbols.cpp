#include "GameSymbols.h"
#include "Log.h"
#include <dlfcn.h>
#include <cstdio>
#include <cstring>
GameSymbols gSymbols;
static uintptr_t FindLibBase(const char* name){ FILE* f=fopen("/proc/self/maps","r"); if(!f) return 0; char line[512]; while(fgets(line,sizeof(line),f)){ if(strstr(line,name)){ uintptr_t start=0; sscanf(line,"%lx-",&start); fclose(f); return start; }} fclose(f); return 0; }
void InitGameSymbols(){ gSymbols.base = FindLibBase("libGTASA.so"); LPD_Log("[SYMBOLS] libGTASA base=%p", (void*)gSymbols.base); LPD_Log("[SYMBOLS] offsets gerados da libGTASA enviada pelo usuário."); }
void* GTASA(uintptr_t offset){ if(!gSymbols.base) InitGameSymbols(); if(!gSymbols.base) return nullptr; return (void*)(gSymbols.base + offset); }
