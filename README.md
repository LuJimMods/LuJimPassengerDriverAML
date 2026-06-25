# LuJim Passenger Driver

Plugin AML para GTA San Andreas Android.

## Estrutura no celular

Copie o conteúdo do artifact para:

```text
Android_unprotected/data/com.rockstargames.gtasa/
```

A estrutura final fica assim:

```text
Android_unprotected/
└── data/
    └── com.rockstargames.gtasa/
        ├── configs/
        │   └── LuJimPassengerDriver/
        │       ├── LuJimPassengerDriver.ini
        │       └── LuJimPassengerDriver.log
        └── mods/
            ├── libLuJimPassengerDriver.so
            └── libLuJimPassengerDriver64.so
```

## Bibliotecas

- `libLuJimPassengerDriver.so` = versão 32 bits / armeabi-v7a.
- `libLuJimPassengerDriver64.so` = versão 64 bits / arm64-v8a.

## Configuração

O mod usa exclusivamente este caminho para configuração e log:

```text
/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/configs/LuJimPassengerDriver/
```

Arquivos:

```text
LuJimPassengerDriver.ini
LuJimPassengerDriver.log
```

Se a pasta ou o arquivo não existirem, o mod cria automaticamente.

## Estado atual

Esta base é segura para teste: os hooks perigosos continuam desativados por padrão com:

```ini
ExperimentalHooks=0
```

Isso permite testar carregamento, criação de pastas, leitura do INI e geração do log antes de ativar hooks avançados.

## Nome

LuJim Passenger Driver

Autor: LuJim Mods
