LuJimPassengerDriverAML - versao para upload pelo Android

Arquivos na raiz:
- main.cpp
- Android.mk
- Application.mk
- build.yml
- README_ANDROID.txt

IMPORTANTE:
No GitHub pelo celular, envie primeiro main.cpp, Android.mk, Application.mk e README_ANDROID.txt.
Depois crie manualmente o arquivo do workflow:
.github/workflows/build.yml

Copie todo o conteudo do arquivo build.yml para dentro desse novo arquivo.
O build.yml que fica na raiz serve apenas para copiar o conteudo.

Depois va em Actions > Build LuJim Passenger Driver AML > Run workflow.

A lib final sera gerada como:
mods/libLuJimPassengerDriverAML.so
