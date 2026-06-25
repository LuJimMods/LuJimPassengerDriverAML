# LuJim Passenger Driver V2.0.0 Clean AML

Base limpa para AML:

- Usa `MYMOD` + `NEEDGAME` oficiais do AML.
- Não usa `mod/logger.cpp`.
- Não usa `mod/config.cpp`.
- Não usa `mod/thirdparty`.
- Usa somente o log próprio `LPD_Log`.
- Mantém suporte 32 bits e 64 bits via `Application.mk` e `build.yml`.

Arquivos da pasta `mod` necessários:

- `mod/amlmod.h`
- `mod/iaml.h`
- `mod/interface.h`

Substitua o projeto por estes arquivos e compile no GitHub Actions.
