#include <mod/amlmod.h>

#include "Log.h"
#include "Config.h"
#include "GameSymbols.h"
#include "PassengerDriver.h"

#include <pthread.h>
#include <unistd.h>

// LuJim Passenger Driver V2.1.0
// Base limpa AML + monitor seguro em thread.
// Esta versão NÃO move o jogador, NÃO controla recruta e NÃO instala hook perigoso.
// Objetivo: confirmar execução contínua do mod dentro do jogo antes de ativar lógica real.
MYMOD(net.lujim.passengerdriver, LuJimPassengerDriver, 2.1.0, LuJimMods)
NEEDGAME(com.rockstargames.gtasa)

static bool g_initialized = false;
static bool g_allModsLoaded = false;
static bool g_updateThreadStarted = false;
static bool g_updateThreadRunning = false;

static void* LPD_UpdateThread(void*)
{
    LPD_Log("[THREAD] Monitor seguro V2.1.0 iniciado");

    // Espera curta para o jogo e os outros mods terminarem a inicializacao.
    sleep(3);

    g_updateThreadRunning = true;

    unsigned int tick = 0;
    while(g_updateThreadRunning)
    {
        // Atualizacao segura: Input ainda nao acessa offsets reais do jogo.
        // PassengerDriver::Update respeita ExperimentalHooks=0 por padrao.
        PassengerDriver::Update(250.0f);

        ++tick;
        if((tick % 40) == 0)
        {
            LPD_Log("[THREAD] Monitor seguro ativo. Enabled=%d ExperimentalHooks=%d AllModsLoaded=%d",
                    LPDSettings::values.enabled ? 1 : 0,
                    LPDSettings::values.experimentalHooks ? 1 : 0,
                    g_allModsLoaded ? 1 : 0);
        }

        usleep(250000); // 250 ms
    }

    LPD_Log("[THREAD] Monitor seguro finalizado");
    return nullptr;
}

static void LPD_StartUpdateThread()
{
    if(g_updateThreadStarted)
    {
        LPD_Log("[THREAD] Monitor seguro ja estava iniciado");
        return;
    }

    pthread_t thread;
    int result = pthread_create(&thread, nullptr, LPD_UpdateThread, nullptr);
    if(result == 0)
    {
        pthread_detach(thread);
        g_updateThreadStarted = true;
        LPD_Log("[THREAD] Monitor seguro criado com sucesso");
    }
    else
    {
        LPD_Log("[THREAD] Falha ao criar monitor seguro. Erro=%d", result);
    }
}

ON_MOD_PRELOAD()
{
    LPD_Log("[AML] OnModPreLoad - LuJim Passenger Driver V2.1.0");
}

ON_MOD_LOAD()
{
    LPD_Log("[AML] OnModLoad - LuJim Passenger Driver V2.1.0 chamado");

    if(g_initialized)
    {
        LPD_Log("[AML] OnModLoad ignorado: mod ja inicializado");
        return;
    }

    g_initialized = true;

    // Inicializacao segura: cria/carrega INI antes de qualquer logica do motorista.
    LPDSettings::EnsureDefault();
    LPDSettings::Load();

    // Apenas localiza base da libGTASA e registra no log.
    // Sem hooks reais nesta etapa.
    InitGameSymbols();
    PassengerDriver::Init();

    LPD_StartUpdateThread();

    LPD_Log("[AML] Inicializacao concluida - LuJim Passenger Driver V2.1.0");
}

ON_ALL_MODS_LOAD()
{
    g_allModsLoaded = true;
    LPD_Log("[AML] OnAllModsLoaded - LuJim Passenger Driver V2.1.0");
}
