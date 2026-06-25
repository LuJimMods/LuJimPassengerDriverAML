# LuJim Passenger Driver

AML plugin for GTA San Andreas Android.

## Estrutura no jogo

Copie o conteudo do artifact para:

`/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/`

Resultado esperado:

```text
com.rockstargames.gtasa/
├── configs/
│   └── LuJimPassengerDriver/
│       ├── LuJimPassengerDriver.ini
│       └── LuJimPassengerDriver.log
└── mods/
    ├── libLuJimPassengerDriver.so
    └── libLuJimPassengerDriver64.so
```

## Bibliotecas

- `libLuJimPassengerDriver.so` = 32 bits / armeabi-v7a
- `libLuJimPassengerDriver64.so` = 64 bits / arm64-v8a

## Configuracao

O mod usa exclusivamente:

`/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs/LuJimPassengerDriver/LuJimPassengerDriver.ini`

O log fica no mesmo diretorio:

`/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs/LuJimPassengerDriver/LuJimPassengerDriver.log`

## Observacao

Esta base ainda mantem os hooks perigosos protegidos por `ExperimentalHooks=0` para evitar crash enquanto a logica completa de entrada pelo passageiro e recruta motorista e implementada.
