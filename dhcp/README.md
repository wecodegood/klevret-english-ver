# DHCP сервер проекта Клеврет

# Протокол DHCP
https://datatracker.ietf.org/doc/html/rfc2131

Опции DHCP:\
https://datatracker.ietf.org/doc/html/rfc2132

# Сборка
Сборка осуществляется с помощью cmake. Для сборки нужно рядом с каталогом сервера "dhcp" создать папку dhcp-build, перейти туда и выполнить команды:
```
cmake ../dhcp
cmake --build .
```
Если нужна отладочная информация в бинарнике, то так:
```
cmake -DCMAKE_BUILD_TYPE=Debug ../dhcp
cmake --build .
```
После чего в этой же папке появится исполняемый файл dhcp-server, который можно запустить
