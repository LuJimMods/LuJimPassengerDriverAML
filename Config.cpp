#include "Config.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sys/stat.h>
LPDConfig LPDSettings::values;
static bool parseBool(const std::string& v){ return v=="1" || v=="true" || v=="TRUE" || v=="on" || v=="ON"; }
static std::string trim(std::string s){
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c){ return !std::isspace(c); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c){ return !std::isspace(c); }).base(), s.end());
    return s;
}
const char* LPDSettings::Path(){ return "/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs/LuJimPassengerDriver/LuJimPassengerDriver.ini"; }
void LPDSettings::EnsureDefault(){
    mkdir("/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs", 0777);
    mkdir("/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs/LuJimPassengerDriver", 0777);
    std::ifstream i(Path());
    if(!i.good()) Save();
}
bool LPDSettings::Load(){
    EnsureDefault();
    std::ifstream f(Path());
    if(!f.good()) return false;
    std::string line;
    while(std::getline(f,line)){
        line=trim(line);
        if(line.empty() || line[0]=='#' || line[0]==';' || line[0]=='[') continue;
        auto p=line.find('=');
        if(p==std::string::npos) continue;
        auto k=trim(line.substr(0,p));
        auto v=trim(line.substr(p+1));
        try{
            if(k=="Enabled") values.enabled=parseBool(v);
            else if(k=="HoldTime") values.holdTimeMs=std::max(500, std::stoi(v));
            else if(k=="ShowNotifications") values.showNotifications=parseBool(v);
            else if(k=="ShowProgressBar") values.showProgressBar=parseBool(v);
            else if(k=="PlaySounds") values.playSounds=parseBool(v);
            else if(k=="NotificationTime") values.notificationTimeMs=std::max(500, std::stoi(v));
            else if(k=="RequireRecruit") values.requireRecruit=parseBool(v);
            else if(k=="UseFirstRecruit") values.useFirstRecruit=parseBool(v);
            else if(k=="UseNearestRecruit") values.useNearestRecruit=parseBool(v);
            else if(k=="PatrolMode") values.patrolMode=parseBool(v);
            else if(k=="RealisticEntry") values.realisticEntry=parseBool(v);
            else if(k=="EnableLogs") values.enableLogs=parseBool(v);
            else if(k=="ExperimentalHooks") values.experimentalHooks=parseBool(v);
        }catch(...){ }
    }
    return true;
}
bool LPDSettings::Save(){
    mkdir("/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs", 0777);
    mkdir("/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs/LuJimPassengerDriver", 0777);
    std::ofstream f(Path());
    if(!f.good()) return false;
    f << "[LuJimPassengerDriver]\n";
    f << "Enabled=" << (values.enabled?1:0) << "\n";
    f << "HoldTime=" << values.holdTimeMs << "\n";
    f << "ShowNotifications=" << (values.showNotifications?1:0) << "\n";
    f << "ShowProgressBar=" << (values.showProgressBar?1:0) << "\n";
    f << "PlaySounds=" << (values.playSounds?1:0) << "\n";
    f << "NotificationTime=" << values.notificationTimeMs << "\n";
    f << "RequireRecruit=" << (values.requireRecruit?1:0) << "\n";
    f << "UseFirstRecruit=" << (values.useFirstRecruit?1:0) << "\n";
    f << "UseNearestRecruit=" << (values.useNearestRecruit?1:0) << "\n";
    f << "PatrolMode=" << (values.patrolMode?1:0) << "\n";
    f << "RealisticEntry=" << (values.realisticEntry?1:0) << "\n";
    f << "EnableLogs=" << (values.enableLogs?1:0) << "\n";
    f << "ExperimentalHooks=" << (values.experimentalHooks?1:0) << "\n\n";
    f << "[AllowedVehicles]\nCars=1\nPolice=1\nBikes=0\nBoats=0\nPlanes=0\nHelicopters=0\nCustomIDs=19001-19200\n\n";
    f << "[Compatibility]\nIgnoreMissionVehicles=1\nIgnoreOccupiedVehicles=1\nDisableDuringMissions=1\nDisableInCutscenes=1\n";
    return true;
}
bool LPDSettings::SaveEnabled(bool enabled){ values.enabled = enabled; return Save(); }
