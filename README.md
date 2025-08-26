#### English:

# klevret

The Klevret Project (from Old Slavic "companion") is a suite of programs for computer network administration.

- [ ] DHCP
- [ ] DNS
- [ ] File server
- [ ] RA VPN

JSON will be used as the API. Frontend - Vue.

## Dependencies:
    boost 1.81

## Build

Requirements:

* CMake >= 3.21
* C++23 compiler

Build process:

1. Create a directory `build`

    ```sh
    mkdir build
    ```

2. Run CMake configuration

    ```sh
    cmake -S . -B build
    ```

3. Build the project

    ```sh
    cmake --build build --config=Release
    ```

If the build is successful, you will find the executables in `./build/<component>/<configuration>/`, for example `./build/core/Release/core-server`.

## Running the tests

When running CMake configuration (step 2 from "Build"), add `-DKLEVRET_BUILD_TESTS=1`, then build as usual.

If the build is successful, test executables will be located at `./build/<component>/tests/<configuration>/`, for example `./build/dhcp/tests/Debug/dhcp-server-tests`.

## Installation

To be added later (?)


---

#### Русский:

# klevret

Проект Клеврет (со старославянского товарищ) - комплекс программ для администрирования компьютерной сети.

- [ ] DHCP
- [ ] DNS
- [ ] Файловый сервер
- [ ] RA VPN

В качестве API будет использоваться JSON. Фронтенд - Vue.

## Зависимости:
    boost 1.81

## Сборка

Требования:

* CMake >= 3.21
* C++23 компилятор

Процесс сборки:

1. Создайте директорию `build`

    ```sh
    mkdir build
    ```

2. Выполните конфигурацию CMake

    ```sh
    cmake -S . -B build
    ```

3. Выполните сборку проекта

    ```sh
    cmake --build build --config=Release
    ```

Если сборка прошла успешно, то исполняемые файлы можно будет найти в `./build/<компонент>/<конфигурация>/`, например `./build/core/Release/core-server`.

## Запуск тестов

При конфигурации CMake (второй шаг из раздела "Сборка") добавить `-DKLEVRET_BUILD_TESTS=1`, после чего выполнить сборку как обычно.

Если сборка прошла успешно, то исполняемые файлы тестов будут расположены по пути `./build/<компонент>/tests/<конфигурация>/`, например `./build/dhcp/tests/Debug/dhcp-server-tests`.

## Установка

Будет добавлена позже (?)
