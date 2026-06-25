# LuJim Passenger Driver - V1 Safe Test

Autor: LuJim Mods

Base AML para GTA San Andreas Android 2.00 / AML 1.3.0.

## O que esta versão faz

- Compila pelo GitHub Actions.
- Carrega no AML com `OnModLoad`.
- Cria/lê `mods/configs/LuJimPassengerDriver.ini`.
- Cria log próprio em `mods/logs/LuJimPassengerDriver.log`.
- Possui sistema de notificações em português preparado.
- Possui sistema de toggle por 3 segundos preparado.

## Segurança

`ExperimentalHooks=0` por padrão.

Isso significa que os hooks perigosos de entrada em veículo, recruta e controle do carro ficam bloqueados até serem testados no jogo. Essa configuração evita crash no primeiro carregamento.

## Instalação

Copie:

`mods/libLuJimPassengerDriver.so`

para:

`/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/mods/`

E copie:

`mods/configs/LuJimPassengerDriver.ini`

para:

`/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/mods/configs/`

Depois abra o jogo e envie:

- `aml.log`
- `mods/logs/LuJimPassengerDriver.log`

## Próxima etapa

Ligar os hooks reais do botão Entrar, depois a sequência de recruta/motorista e jogador/passageiro.
